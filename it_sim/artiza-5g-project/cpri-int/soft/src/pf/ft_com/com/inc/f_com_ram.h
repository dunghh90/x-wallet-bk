/*******************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_ram.c
 *  @brief  RAM Table for common function
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 *  @date   2009/04/16  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 *  @date   2009/06/09  FFCS)Wuh modify for M-S3G-eNBPF-02047
 *                      RE version response incorrect
 *	@date	2009/06/10  FFCS)Wangjuan modify for M-S3G-eNBPF-02050
 *						BRE card can not connect in SV-MT/FLD-MT debug test
 *  @date   2009/06/16  FFCS)Wuh modify for M-S3G-eNBPF-02086, 
 *                      modify for FLASH erase in file no new case faild
 *  @date   2009/08/10  FFCS)Niemsh modify for CR-00054-005, 
 *                      add common variable:cmw_ftpulstpflg,cmw_ftpulmaxmsgq
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2014/12/05 ALPHA)okabe Modify
 *  @date   2018/02/14 FJT)吉田 add 5G-DU対応
 *  @date   2018/02/23 FJT)吉田 modify
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2018
 */
/*******************************************************************************/


/** @addtogroup RRH_PF_COM
 *  @{
 */
#ifndef F_COM_RAM_H
#define F_COM_RAM_H

/******************************************************************************/
/* include file
 */
/******************************************************************************/

/****************************************************************************/
/*																			*/
/*	Common table declaration												*/
/*																			*/
/****************************************************************************/

/** @name system parameter management table */
/* @{ */

/* system parameter management table									*/
/* cmw_sys_mng_tbl[0]: 3G system parameter management					*/
/* cmw_sys_mng_tbl[1]: S3G system parameter management					*/

extern T_RRH_SYSMNGTBL*			f_cmw_sys_mng_tbl[2];
							/**< System parameter management table  */

/* @} */


/** @name inventory management table */
/* @{ */

extern T_RRH_TRAINVTBL*		f_cmw_tra_inv_tbl;		/**< TRA card inventory management table  */

/* 2020/12/28 M&C) Merge 4G FHM src (start remove (reason: duplicate declaration)) */
// extern T_RRH_SHELFINVTBL*		cmw_slf_inv_tbl;			/**< shelf inventory information management table  */
/* 2020/12/28 M&C) Merge 4G FHM src (end remove) */

/* @} */

/** @name external interface common management table */
/* @{ */

/* @} */

/** @name RE device management table */
/* @{ */

extern T_RRH_REDEVCOMPTBL*			f_cmw_re_dev_tbl;				/**< RE device management table  */

/* ======================================================================== */
/* DU接続情報 (DBG)															*/
/* ======================================================================== */
extern	T_COM_DU_CNCT_DBG_TBL		f_cmw_du_cnct_dbg_tbl;

/* @} */

/** @name Cascade information table */
/* @{ */

extern T_RRH_CASCADEINFOTBL*		f_cmw_cascade_info_tbl;			/**< Cascade information table */

/* @} */


/** @name fldc4 compress related */
/* @{ */
extern UCHAR 					cmw_devdata_tbl[0xC00000];
extern FLDC4_ELEM				cmw_fa_dictionary;			/**< fldc4 compress dictionary  */
extern UCHAR					cmw_fa_temporary[32*1024+2] ;
						/**< fldc4 compress temporary area  */
/* @} */


/** @name receive firmware data table */
/* @{ */
extern UCHAR	*			f_cmw_rcvfirm_tbl; /* ƒf[ƒ^Ši”[ƒe[ƒuƒ‹		*/
/* @} */

/** @name system parameter management table */
/* @{ */

/**@used for hardware alarm factor&mask table */
/* @{ */
extern UINT f_cmw_hardalm_factor;
extern UINT f_cmw_hardalm_mask;
/* @} */

/**@used for hardware alarm factor&mask table */
/* @{ */
extern UINT 	f_comw_hardalm_mask;
/* @} */

/**@used for hardware alarm factor&mask table */
/* @{ */
extern UINT f_comw_hardalm_factor_1chp;
extern UINT f_comw_hardalm_mask_1chp;
/* @} */

/** @name RSV thread state */
/* @{ */
extern T_RRH_SVSTATELOG		f_comw_svStateLog;
/* @} */

extern	UINT				f_comw_rrhKind;								/**< 装置種別設定								*/
extern  UINT                f_comw_reKind;
/* SRE add- Start */
extern	UINT				f_comw_init_rrhKind;						/**< 装置種別設定(initプロセス用)				*/
extern	UINT 				f_comw_hard_rxtraining_sre_flg;				/**< RX側ﾄﾚｰﾆﾝｸﾞﾃﾞｰﾀ(ハード)完了フラグ			*/
/* SRE add- End */
extern	UINT				f_comw_rrhVersion;							/**< RRH Version								*/
extern	UINT				f_comw_BoardVer;							/**< Board Version								*/
extern	T_RRH_TRAINING_DATA	*f_comw_trainingData;						/**< training data								*/
extern	T_RRH_RF_TRAINING_DATA	*f_comw_rftrainingData;					/**< training data								*/ /* 5GDU_chg */
extern	UCHAR				f_comw_orgData[D_RRH_FLASH_LOG_SIZE_BEFORE];/**< 圧縮データ用buff addr						*/
extern	T_COM_LOG_COMPRESS_TBL	f_comw_compressData_cycle;				/**< サイクリックログ圧縮データ					*/
extern	FLDC4_ELEM			f_comw_fa_dictionary;						/**< fldc4 compress dictionary					*/
extern	UCHAR				*f_comw_trainingTx;							/**< TrainingData(Tx)初期値						*/
extern	UINT				f_comw_cpriFpgaChkSumDoneFlag;				/**< CPRI-FPGA チェックサム計算済みフラグ		*/
extern	UINT				f_comw_cpriFpgaChkSumVal;					/**< CPRI-FPGA チェックサム計算値				*/
extern	UINT				f_comw_fwVatt_data[D_COM_FWVATT_DATA_NUM];	/**< FWVATT設定データ							*/
extern	UINT				f_comw_paDac_data[D_COM_PADAC_DATA_NUM];	/**< PADAC設定データ							*/

extern	T_RRH_DU_INFORMATION_EEPROM		*f_comw_du_netinfo_eep_tbl;		/**< DUネットワーク情報(EEPROM)					*/
extern	T_RRH_DU_INFORMATION_FLASH		*f_comw_searchable_vlan_info;	/**< DUネットワーク情報(FLASH)					*/
extern	T_RRH_DU_CONNECTION_INFO		*f_comw_du_connection_tbl;		/**< DUコネクション情報							*/

extern	T_RRH_NETCONF_INFO_TBL			*f_comw_netconf_tbl;			/**< NETCONFユーザー情報テーブル				*/
extern	T_RRH_PTPCONF_MNG				*f_comw_ptpconf_tbl;			/**< PTP Configurationテーブル					*/

extern	T_RRH_CAL_COEF_TBL				*f_comw_txcal_coef_tbl;			/**< TX CAL係数テーブル							*/
extern	T_RRH_CAL_COEF_TBL				*f_comw_rxcal_coef_tbl;			/**< RX CAL係数テーブル							*/

extern	T_RRH_PFM_PERFORMANCE_TBL		*f_comw_pfmlog_tbl;				/**< Performanceログテーブル					*/
extern	T_RRH_PFM_PERFORMANCE_MNG_TBL	*f_comw_pfmmng_tbl;				/**< Performance情報管理テーブル				*/

extern	T_RRH_SFNFRM_CALB_TBL			*f_comw_sfnfrm_tbl;				/**< SFN/FRM補正用テーブル						*/
extern	T_RRH_LBM_MNGMENT_TBL			*f_comw_lbm_mngment_tbl;		/**< LBM/LBR管理テーブル						*/
extern	T_RRH_BF_MODE_TBL				*f_comw_bf_mode;				/**< Beamforming Mode Table						*/

/** @name RSV thread state */
/* @{ */
extern T_RRH_SVSTATECHG_HIS*		f_comw_svStateChg_his;
/* @} */

/** @ WDT Hndl Flg */
/* @{ */
extern UINT f_comw_wdtHndlFlg;
/* @} */

/** @ AISG */
/* @{ */
extern T_COM_AISGDEV	f_comw_aisgDev[D_SYS_AISG_MAX_DEVNUM];	        /**< AISG Dev Table  */
extern T_COM_AISGSTATICS 	f_comw_aisgStatics;	/**< AISG statistics Table  */
UINT						f_comw_aisgDevNum;		/* AISG Device number */

extern UINT					f_comw_aisgSendQFull;	/* AISG Send Q Full or not */
/* @} */
/** @ SFP */
/* @{ */
extern		UINT			f_comw_sfpType;						/**< SFPタイプ SRE時とRRE時のPrimary用として使用	*/
extern		UINT			f_comw_sfpTypeSecondary;			/**< SFPタイプ RRE時のSecondary用として使用	*/
/* @} */

/** @name log table */
/* @{ */
extern T_RRH_LOG_WDT		* f_comw_wdt_log;
extern T_RRH_LOG_ECC		* f_comw_ecc_log;
/* @} */

/* FLASH EOL対応 add start */
/* @ Manufacture ID */
/* @{ */
extern UINT					f_comw_plManufactureId;				/**< PL側 Manufacture ID	*/
extern UINT					f_comw_psManufactureId;				/**< PS側 Manufacture ID	*/
extern UINT					f_comw_reManufactureId[4];			/**< RE側 Manufacture ID	*/
/* @} */
/* FLASH EOL対応 add end */


/* @name Interface Name */
/* @{ */
extern 	CHAR				f_com_ifname[D_RRH_IF_NAME_SMAX];
/* @} */

/* @name product-code */
/* @{ */
extern 	CHAR				f_com_product_code[16];
/* @} */

/*!
 * @brief SV Bus control mapping Table
 * @{
 */
/*	SV Bus control mapping table	*/
extern 	T_COM_DU_SVBUSCTRL_MAP_TBL f_comw_svOptMapTbl[E_DU_SVCTL_OPE_MAX];
T_RRH_DU_FAULT_MNG	*f_comw_fltMngTblp;

extern	UINT	f_comw_rf_setup_flg;

/**
 * @brief	Structure of SFP monitor
 * @note	Structure of SFP monitor table
 * {@
 */
extern	T_COM_TRANSCEIVER_MONITOR_TBL	f_comw_transcelver_mon_tbl;		/**< SFP monitor テーブル	*/
/* @} */

/**
 * @brief	Structure of restart_time table
 * @note	Structure of restart_time table
 * {@
 */
extern	T_RRH_SYSTEM_TIME				f_comw_operations_restart_time;	/**< SFP monitor テーブル	*/
/* @} */


/**
 * @brief	Supervison start flag
 * @note	Supervison start flag
 * {@
 */
extern	UINT	f_comw_mp_supervison_flg;
/* @} */

#endif
/* @} */
