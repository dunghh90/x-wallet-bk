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
 *  @date   2018/02/14 FJT)吉田 add 5G-DU対応
 *  @date   2018/02/23 FJT)吉田 modify
 * 
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2018
 */
/*******************************************************************************/

/** @addtogroup RRH_PF_COM
 *  @{
 */

/******************************************************************************/
/* include file
 */
/******************************************************************************/
#include "f_com_inc.h"
 	
/* matsumoto modify start */
#ifdef D_RRH_MODE_REC
#include "f_rrh_reg_eep.h"
#endif
/* matsumoto modify end */

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

T_RRH_SYSMNGTBL*			f_cmw_sys_mng_tbl[2];
							/**< System parameter management table  */

/* @} */


/** @name inventory management table */
/* @{ */

T_RRH_TRAINVTBL*		f_cmw_tra_inv_tbl;		/**< TRA card inventory management table  */

T_RRH_SHELFINVTBL*		f_cmw_slf_inv_tbl;		/**< shelf inventory information management table  */

/* @} */

/** @name external interface common management table */
/* @{ */

/* @} */

/** @name RE device management table */
/* @{ */

T_RRH_REDEVCOMPTBL*			f_cmw_re_dev_tbl;				/**< RE device management table  */

/* @} */

/* ======================================================================== */
/* DU接続情報 (DBG)															*/
/* ======================================================================== */
T_COM_DU_CNCT_DBG_TBL		f_cmw_du_cnct_dbg_tbl;

/** @name Cascade information table */
/* @{ */

T_RRH_CASCADEINFOTBL*		f_cmw_cascade_info_tbl;			/**< Cascade information table */

/* @} */

/** @name fldc4 compress related */
/* @{ */
UCHAR 					cmw_devdata_tbl[0xC00000];
FLDC4_ELEM				cmw_fa_dictionary;			/**< fldc4 compress dictionary  */
UCHAR					cmw_fa_temporary[32*1024+2] ;
						/**< fldc4 compress temporary area  */
/* @} */


/** @name receive firmware data table */
/* @{ */
UCHAR					*f_cmw_rcvfirm_tbl; /* データ格納テーブル		*/
/* @} */

UINT					f_comw_rrhKind;								/**< 装置種別設定								*/
UINT					f_comw_reKind;								/**< RE種別										*/
/* SRE対応 - Start */
UINT					f_comw_init_rrhKind;						/**< 装置種別設定(initプロセス用)				*/
UINT 					f_comw_hard_rxtraining_sre_flg;				/**< RX側ﾄﾚｰﾆﾝｸﾞ完了フラグ						*/
/* SRE対応 - End */
UINT					f_comw_rrhVersion;							/**< RRH Version								*/
UINT					f_comw_BoardVer;							/**< Board Version								*/
T_RRH_TRAINING_DATA		*f_comw_trainingData;						/**< training data								*/
T_RRH_RF_TRAINING_DATA	*f_comw_rftrainingData;						/**< RF用 training data								*/ /* 5GDU_chg */
UCHAR					f_comw_orgData[D_RRH_FLASH_LOG_SIZE_BEFORE];/**< 圧縮データ用元データ						*/
T_COM_LOG_COMPRESS_TBL	f_comw_compressData_cycle;					/**< サイクリックログ圧縮データ					*/
FLDC4_ELEM				f_comw_fa_dictionary;						/**< fldc4 compress dictionary					*/
UCHAR					*f_comw_trainingTx;							/**< TrainingData(Tx)初期値						*/
UINT					f_comw_cpriFpgaChkSumDoneFlag;				/**< CPRI-FPGA チェックサム計算済みフラグ		*/
UINT					f_comw_cpriFpgaChkSumVal;					/**< CPRI-FPGA チェックサム計算値				*/
UINT					f_comw_fwVatt_data[D_COM_FWVATT_DATA_NUM];	/**< FWVATT設定データ							*/
UINT					f_comw_paDac_data[D_COM_PADAC_DATA_NUM];	/**< PADAC設定データ							*/

T_RRH_DU_INFORMATION_EEPROM		*f_comw_du_netinfo_eep_tbl;			/**< DUネットワーク情報(EEPROM)					*/
T_RRH_DU_INFORMATION_FLASH		*f_comw_searchable_vlan_info;		/**< DUネットワーク情報(FLASH)					*/
T_RRH_DU_CONNECTION_INFO		*f_comw_du_connection_tbl;			/**< DUコネクション情報							*/

T_RRH_NETCONF_INFO_TBL			*f_comw_netconf_tbl;				/**< NETCONFユーザー情報テーブル				*/
T_RRH_PTPCONF_MNG				*f_comw_ptpconf_tbl;				/**< PTP Configurationテーブル					*/

T_RRH_CAL_COEF_TBL				*f_comw_txcal_coef_tbl;				/**< TX CAL係数テーブル							*/
T_RRH_CAL_COEF_TBL				*f_comw_rxcal_coef_tbl;				/**< RX CAL係数テーブル							*/

T_RRH_PFM_PERFORMANCE_TBL		*f_comw_pfmlog_tbl;					/**< Performanceログテーブル					*/
T_RRH_PFM_PERFORMANCE_MNG_TBL	*f_comw_pfmmng_tbl;					/**< Performance情報管理テーブル				*/

T_RRH_SFNFRM_CALB_TBL			*f_comw_sfnfrm_tbl;					/**< SFN/FRM補正用テーブル						*/
T_RRH_LBM_MNGMENT_TBL			*f_comw_lbm_mngment_tbl;			/**< LBM/LBR管理テーブル						*/
T_RRH_BF_MODE_TBL				*f_comw_bf_mode;					/**< Beamforming Mode Table						*/

/**@used for hardware alarm factor&mask table */
/* @{ */
UINT 	f_comw_hardalm_mask;
/* @} */

/**@used for hardware alarm factor&mask table */
/* @{ */
UINT f_comw_hardalm_factor_1chp;
UINT f_comw_hardalm_mask_1chp;
/* @} */

/** @name RSV thread state */
/* @{ */
T_RRH_SVSTATELOG		f_comw_svStateLog;
/* @} */

/** @name RSV thread state */
/* @{ */
T_RRH_SVSTATECHG_HIS*		f_comw_svStateChg_his = NULL;
/* @} */

/* matsumoto modify start */
#ifdef D_RRH_MODE_REC
T_RRH_TRAINVTBL			cmw_tra_inv_tbl;		/**< TRA card inventory management table  */
#endif
/* matsumoto modify end */

T_COM_AISGDEV			f_comw_aisgDev[D_SYS_AISG_MAX_DEVNUM];	        /**< AISG Dev Table  */
T_COM_AISGSTATICS 		f_comw_aisgStatics;
UINT						f_comw_aisgDevNum;		/* AISG Device number */
UINT						f_comw_aisgSendQFull;	/* AISG Send Q Full or not */

/** @ WDT Hnd Flg 
 *  @{
 */
UINT     f_comw_wdtHndlFlg;       /**< WDT(IP Macro)割り込みFlag   */
/* @} */

/* @ SFP */
/* @{ */
UINT				f_comw_sfpType;						/**< SFPタイプ SRE機とRRE機のPrimary用	*/
UINT				f_comw_sfpTypeSecondary;			/**< SFPタイプ RRE機のSecondary用 		*/
/* @} */

/** @name log table */
/* @{ */
T_RRH_LOG_WDT		* f_comw_wdt_log;
T_RRH_LOG_ECC		* f_comw_ecc_log;
/* @} */

/* FLASH EOL対応 add start */
/* @ Manufacture ID */
/* @{ */
UINT				f_comw_plManufactureId;				/**< PL側 Manufacture ID	*/
UINT				f_comw_psManufactureId = 0;			/**< PS側 Manufacture ID	*/
UINT				f_comw_reManufactureId[4];			/**< RE側 Manufacture ID	*/
/* @} */
/* FLASH EOL対応 add end */

/* @name Interface Name */
/* @{ */
CHAR				f_com_ifname[D_RRH_IF_NAME_SMAX];
/* @} */

/* @name product-code */
/* @{ */
CHAR				f_com_product_code[16];
/* @} */

/*!
 * @brief SV Bus control mapping Table
 * @{
 */
T_COM_DU_SVBUSCTRL_MAP_TBL f_comw_svOptMapTbl[E_DU_SVCTL_OPE_MAX] = 
	{	/*     info TBL                                           , size                           , ope type          , mask val                       , register addr       , shift info , def        */
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptAdmTbl     , sizeof(f_comr_svOptAdmTbl)     , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_BIT_ADM_ALL         , D_DU_REG_CTB_SV1INF , D_DU_SVCTL_ON_ADM_SFT        , D_DU_SVCTL_ON_ADM_UNLOCKED },		/*	(00)E_DU_SVCTL_OPE_ADM			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptEngTbl     , sizeof(f_comr_svOptEngTbl)     , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_BIT_ENG_ALL         , D_DU_REG_CTB_SV1INF , D_DU_SVCTL_ON_ENG_SFT        , D_DU_SVCTL_ON_ENG_AWAKE },		/*	(01)E_DU_SVCTL_OPE_ENG			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptOpeTbl     , sizeof(f_comr_svOptOpeTbl)     , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_BIT_OPE_ALL         , D_DU_REG_CTB_SV1INF , D_DU_SVCTL_ON_OPE_SFT        , D_DU_SVCTL_ON_OPE_DISABLE },		/*	(02)E_DU_SVCTL_OPE_OPE			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptAvlTbl     , sizeof(f_comr_svOptAvlTbl)     , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_BIT_AVL_ALL         , D_DU_REG_CTB_SV1INF , D_DU_SVCTL_ON_AVL_SFT        , D_DU_SVCTL_ON_AVL_NORMAL },		/*	(03)E_DU_SVCTL_OPE_AVL			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUsaTbl     , sizeof(f_comr_svOptUsaTbl)     , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_BIT_USA_ALL         , D_DU_REG_CTB_SV1INF , D_DU_SVCTL_ON_USA_SFT        , D_DU_SVCTL_ON_USA_IDLE },		/*	(04)E_DU_SVCTL_OPE_USA			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUseAsTbl   , sizeof(f_comr_svOptUseAsTbl)   , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_BIT_USESLP_A_ALL_SB6, D_DU_REG_CTB_SV2INF , D_DU_SVCTL_ON_USE_A_SFT      , D_DU_SVCTL_ON_OFF_ALL },		/*	(05)E_DU_SVCTL_OPE_USE_A		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUseBsTbl   , sizeof(f_comr_svOptUseBsTbl)   , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_BIT_USESLP_B_ALL_SB6, D_DU_REG_CTB_SV2INF , D_DU_SVCTL_ON_USE_B_SFT      , D_DU_SVCTL_ON_OFF_ALL },		/*	(06)E_DU_SVCTL_OPE_USE_B		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUseCTbl    , sizeof(f_comr_svOptUseCTbl)    , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_BIT_USESLP_C_ALL_SB6, D_DU_REG_CTB_SV2INF , D_DU_SVCTL_ON_USE_C_SFT      , D_DU_SVCTL_ON_OFF_ALL },		/*	(07)E_DU_SVCTL_OPE_USE_C		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUseDTbl    , sizeof(f_comr_svOptUseDTbl)    , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_BIT_USESLP_D_ALL_SB6, D_DU_REG_CTB_SV2INF , D_DU_SVCTL_ON_USE_D_SFT      , D_DU_SVCTL_ON_OFF_ALL },		/*	(08)E_DU_SVCTL_OPE_USE_D		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFlvTbl     , sizeof(f_comr_svOptFlvTbl)     , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_BIT_FLV_ALL         , D_DU_REG_CTB_SV3INF , D_DU_SVCTL_ON_FLV_SFT        , D_DU_SVCTL_ON_OFF_ALL },		/*	(09)E_DU_SVCTL_OPE_FLV			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFidTbl     , sizeof(f_comr_svOptFidTbl)     , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_BIT_OCCUR_FID_ALL   , D_DU_REG_CTB_SV4INF , D_DU_SVCTL_ON_OCCUR_FID_SFT  , D_DU_SVCTL_ON_OFF_ALL },		/*	(10)E_DU_SVCTL_OPE_FID			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid01Tbl   , sizeof(f_comr_svOptFid01Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT01_ALL          , D_DU_REG_CTB_SV5INF , D_DU_FLTID_01_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(11)E_DU_SVCTL_OPE_SFT_ALMCD	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid02Tbl   , sizeof(f_comr_svOptFid02Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT02_ALL          , D_DU_REG_CTB_SV5INF , D_DU_FLTID_02_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(12)E_DU_SVCTL_OPE_INI_ALMCD	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid03Tbl   , sizeof(f_comr_svOptFid03Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT03_ALL          , D_DU_REG_CTB_SV6INF , D_DU_FLTID_03_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(13)E_DU_SVCTL_OPE_FID03_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid04sTbl  , sizeof(f_comr_svOptFid04sTbl)  , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT04_ALL_SB6      , D_DU_REG_CTB_SV6INF , D_DU_FLTID_04_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(14)E_DU_SVCTL_OPE_FID04_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid05Tbl   , sizeof(f_comr_svOptFid05Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT05_ALL          , D_DU_REG_CTB_SV6INF , D_DU_FLTID_05_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(15)E_DU_SVCTL_OPE_FID05_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid06Tbl   , sizeof(f_comr_svOptFid06Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT06_ALL          , D_DU_REG_CTB_SV6INF , D_DU_FLTID_06_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(16)E_DU_SVCTL_OPE_FID06_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid07sTbl  , sizeof(f_comr_svOptFid07sTbl)  , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT07_ALL_SB6      , D_DU_REG_CTB_SV7INF , D_DU_FLTID_07_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(17)E_DU_SVCTL_OPE_FID07_ALM	*//*	(17)E_DU_SVCTL_OPE_FID07LL_ALM, E_DU_SVCTL_OPE_FID07T1_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid08Tbl   , sizeof(f_comr_svOptFid08Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT08_ALL_SB6      , D_DU_REG_CTB_SV8INF , D_DU_FLTID_08_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(18)E_DU_SVCTL_OPE_FID08_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10AntAsTbl, sizeof(f_comr_svOptF10AntAsTbl), D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT10_A_ALL_SB6    , D_DU_REG_CTB_SV9INF , D_DU_FLTID_10_A_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(19)E_DU_SVCTL_OPE_FID10_ALM_A	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10AntBsTbl, sizeof(f_comr_svOptF10AntBsTbl), D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT10_B_ALL_SB6    , D_DU_REG_CTB_SV9INF , D_DU_FLTID_10_B_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(20)E_DU_SVCTL_OPE_FID10_ALM_B	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10AntCTbl , sizeof(f_comr_svOptF10AntCTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT10_C_ALL_SB6    , D_DU_REG_CTB_SV10INF, D_DU_FLTID_10_C_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(21)E_DU_SVCTL_OPE_FID10_ALM_C	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10AntDTbl , sizeof(f_comr_svOptF10AntDTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT10_D_ALL_SB6    , D_DU_REG_CTB_SV10INF, D_DU_FLTID_10_D_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(22)E_DU_SVCTL_OPE_FID10_ALM_D	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF11AntAsTbl, sizeof(f_comr_svOptF11AntAsTbl), D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT11_A_ALL_SB6    , D_DU_REG_CTB_SV9INF , D_DU_FLTID_11_A_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(23)E_DU_SVCTL_OPE_FID11_ALM_A	*//*	(23)E_DU_SVCTL_OPE_FID20_ALM_A	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF11AntBsTbl, sizeof(f_comr_svOptF11AntBsTbl), D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT11_B_ALL_SB6    , D_DU_REG_CTB_SV9INF , D_DU_FLTID_11_B_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(24)E_DU_SVCTL_OPE_FID11_ALM_B	*//*	(24)E_DU_SVCTL_OPE_FID20_ALM_B	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF11AntCTbl , sizeof(f_comr_svOptF11AntCTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT11_C_ALL_SB6    , D_DU_REG_CTB_SV10INF, D_DU_FLTID_11_C_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(25)E_DU_SVCTL_OPE_FID11_ALM_C	*//*	(25)E_DU_SVCTL_OPE_FID20_ALM_C	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF11AntDTbl , sizeof(f_comr_svOptF11AntDTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT11_D_ALL_SB6    , D_DU_REG_CTB_SV10INF, D_DU_FLTID_11_D_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(26)E_DU_SVCTL_OPE_FID11_ALM_D	*//*	(26)E_DU_SVCTL_OPE_FID20_ALM_D	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF12AntATbl , sizeof(f_comr_svOptF12AntATbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT12_A_ALL_SB6    , D_DU_REG_CTB_SV9INF , D_DU_FLTID_12_A_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(27)E_DU_SVCTL_OPE_FID12_ALM_A	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF12AntBTbl , sizeof(f_comr_svOptF12AntBTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT12_B_ALL_SB6    , D_DU_REG_CTB_SV9INF , D_DU_FLTID_12_B_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(28)E_DU_SVCTL_OPE_FID12_ALM_B	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF12AntCTbl , sizeof(f_comr_svOptF12AntCTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT12_C_ALL_SB6    , D_DU_REG_CTB_SV10INF, D_DU_FLTID_12_C_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(29)E_DU_SVCTL_OPE_FID12_ALM_C	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF12AntDTbl , sizeof(f_comr_svOptF12AntDTbl) , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT12_D_ALL_SB6    , D_DU_REG_CTB_SV10INF, D_DU_FLTID_12_D_SFT          , D_DU_SVCTL_ON_OFF_ALL },		/*	(30)E_DU_SVCTL_OPE_FID12_ALM_D	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10VHTbl   , 0                              , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT10_VH_ALL_SB6   , D_DU_REG_CTB_SV11INF, D_DU_FLTID_10_VH_SFT         , D_DU_SVCTL_ON_OFF_ALL },		/*	(31)E_DU_SVCTL_OPE_FID10_ALM_VH	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid30Tbl   , sizeof(f_comr_svOptFid30Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT30_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_30_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(32)E_DU_SVCTL_OPE_FID30_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid31Tbl   , sizeof(f_comr_svOptFid31Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT31_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_31_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(33)E_DU_SVCTL_OPE_FID31_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid40Tbl   , sizeof(f_comr_svOptFid40Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT40_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_40_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(34)E_DU_SVCTL_OPE_FID40_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid41Tbl   , sizeof(f_comr_svOptFid41Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT41_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_41_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(35)E_DU_SVCTL_OPE_FID41_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid42Tbl   , sizeof(f_comr_svOptFid42Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT42_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_42_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(36)E_DU_SVCTL_OPE_FID42_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid43Tbl   , sizeof(f_comr_svOptFid43Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT43_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_43_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(37)E_DU_SVCTL_OPE_FID43_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid44Tbl   , sizeof(f_comr_svOptFid44Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT44_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_44_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(38)E_DU_SVCTL_OPE_FID44_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid50Tbl   , sizeof(f_comr_svOptFid50Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT50_ALL          , D_DU_REG_CTB_SV13INF, D_DU_FLTID_50_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(39)E_DU_SVCTL_OPE_FID50_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFidf0Tbl   , sizeof(f_comr_svOptFidf0Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLTF0_ALL          , D_DU_REG_CTB_SV14INF, D_DU_FLTID_F0_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(40)E_DU_SVCTL_OPE_FIDF0_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFidf1Tbl   , sizeof(f_comr_svOptFidf1Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLTF1_ALL          , D_DU_REG_CTB_SV14INF, D_DU_FLTID_F1_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(41)E_DU_SVCTL_OPE_FIDF1_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFidf2Tbl   , sizeof(f_comr_svOptFidf2Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLTF2_ALL          , D_DU_REG_CTB_SV14INF, D_DU_FLTID_F2_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(42)E_DU_SVCTL_OPE_FIDF2_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFidffTbl   , sizeof(f_comr_svOptFidffTbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLTFF_ALL          , D_DU_REG_CTB_SV14INF, D_DU_FLTID_FF_SFT            , D_DU_SVCTL_ON_OFF_ALL },		/*	(43)E_DU_SVCTL_OPE_FIDFF_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFlDwnldTbl , sizeof(f_comr_svOptFlDwnldTbl) , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_FL_DWNLD_ALL        , D_DU_REG_CTB_SV15INF, D_DU_SVCTL_FL_DWNLD_SFT      , D_DU_SVCTL_ON_OFF_ALL },		/*	(44)E_DU_SVCTL_OPE_FLDL			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptStpWTbl    , sizeof(f_comr_svOptStpWTbl)    , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_WAVE_STOP           , D_DU_REG_CTB_SV16INF, D_DU_SVCTL_ON_WAVE_STOP_SFT  , D_DU_SVCTL_ON_OFF_ALL },		/*	(45)E_DU_SVCTL_OPE_STP_WAV		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptSyncETbl   , sizeof(f_comr_svOptSyncETbl)   , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_SYNCE_STATE_ALL     , D_DU_REG_CTB_SV16INF, D_DU_SVCTL_ON_SYNCE_STATE_SFT, D_DU_SVCTL_ON_SYNCE_STATE_DISABLE },		/*	(46)E_DU_SVCTL_OPE_SYNCE		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptSplanTbl   , sizeof(f_comr_svOptSplanTbl)   , D_DU_SVCTL_TYP_OTH, D_DU_SVCTL_SPLAN_STS_ALL       , D_DU_REG_CTB_SV16INF, D_DU_SVCTL_ON_SPLAN_SFT      , D_DU_SVCTL_ON_OFF_ALL },		/*	(47)E_DU_SVCTL_OPE_SPLAN		*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptSyncTbl    , sizeof(f_comr_svOptSyncTbl)    , D_DU_SVCTL_TYP_STA, D_DU_SVCTL_SYNC_STATE_ALL      , D_DU_REG_CTB_SV16INF, D_DU_SVCTL_ON_SYNC_STATE_SFT , D_DU_SVCTL_ON_SYNC_STATE_FREERUN },		/*	(48)E_DU_SVCTL_OPE_SYNC			*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid51Tbl   , sizeof(f_comr_svOptFid51Tbl)   , D_DU_SVCTL_TYP_FLT, D_DU_ALMBIT_FLT51ALM_ALL       , D_DU_REG_CTB_SV13INF, D_DU_FLTID_51ALM_SFT         , D_DU_SVCTL_ON_OFF_ALL },		/*	(49)E_DU_SVCTL_OPE_FID51_ALM	*/
		{ (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid51NoTbl , sizeof(f_comr_svOptFid51NoTbl) , D_DU_SVCTL_TYP_OTH, D_DU_ALMBIT_FLT51NO_ALL        , D_DU_REG_CTB_SV13INF, D_DU_FLTID_51NO_SFT          , D_DU_SVCTL_ON_OFF_ALL }		/*	(50)E_DU_SVCTL_OPE_FID51_NOALM	*/
	};
/* @} */

/*!
 * @fault occur num manage table
 * @{
 */
/*	fault level occur num manage table pointer	*/
T_RRH_DU_FAULT_MNG	*f_comw_fltMngTblp = NULL;
/* @} */

UINT	f_comw_rf_setup_flg = D_RRH_OFF;

/**
 * @brief	Structure of SFP monitor
 * @note	Structure of SFP monitor table
 * {@
 */
T_COM_TRANSCEIVER_MONITOR_TBL	f_comw_transcelver_mon_tbl;		/**< SFP monitor テーブル	*/
/* @} */

/**
 * @brief	Structure of restart_time table
 * @note	Structure of restart_time table
 * {@
 */
T_RRH_SYSTEM_TIME				f_comw_operations_restart_time;	/**< SFP monitor テーブル	*/
/* @} */

/**
 * @brief	Supervison start flag
 * @note	Supervison start flag
 * {@
 */
UINT	f_comw_mp_supervison_flg = D_RRH_OFF;
/* @} */

/**
 * @brief	default user disabled flag
 * @note	default user disabled flag
 * {@
 */
UINT	f_comw_default_user_disabled_flag = D_RRH_OFF;
/* @} */

/**
 * @brief	dump mng table
 * @note	dump mng table
 * {@
 */
T_COM_DUMP_MNG	f_comw_dumpmsg;
/* @} */

/* @} */
