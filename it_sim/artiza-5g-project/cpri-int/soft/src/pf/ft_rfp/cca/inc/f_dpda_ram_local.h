/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	External Definition
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(dpda)_ram.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2015
 */
/******************************************************************************************************************************/

#include "f_dpda_inc.h"
#include "f_dpda_ram.h"

#ifndef	F_DPDA_RAM_LOCAL_H
#define	F_DPDA_RAM_LOCAL_H

/** @addtogroup RRH_PF_RFP
 *  @{
 */

extern T_DPDA_ANTCARSTATEINFO	f_dpdaw_antCarStateInfo __attribute__ ((aligned (32)));		/**< ant/carrier state info table				*/
extern T_DPDA_LOG_CAR_STATE	*f_dpdaw_carStateLog;

extern T_DPDA_REGDATA			f_dpdaw_regdataReal __attribute__ ((aligned (32)));			/**< register data table						*/
extern T_DPDA_EXTDATA			f_dpdaw_extdataReal __attribute__ ((aligned (32)));
extern T_DPDA_STATUS_INFO		f_dpdaw_statusInfo __attribute__ ((aligned (32)));			/**< status information テーブル				*/
extern T_DPDA_EXT_STATUS_INFO	f_dpdaw_statusInfoExt __attribute__ ((aligned (32)));		/**< status information テーブル (拡張領域)		*/

extern UINT						f_dpdaw_almThreshold[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];		/**< alm threshold table				*/
extern UINT						f_dpdaw_occurAlmInstanceId[D_DPDA_ALM_NUM];					/**< 発生アラームInstanceIDリスト				*/
extern UINT						f_dpdaw_clearAlmInstanceId[D_DPDA_ALM_NUM];					/**< 回復アラームInstanceIDリスト				*/
extern T_DPDA_ALM_STATE			f_dpdaw_almState[D_DPDA_ALM_NUM];							/**< dpda alarm state table						*/
extern UINT						f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_NUM];		/**< dpda alarm collect sw modify info table	*/
extern T_DPDA_ALM_INFO			*f_dpdaw_almInfo;											/**< dpda alarm information table				*/

extern T_DPDA_INFO_ALL			*f_dpdaw_info_all;											/**< DPDAデバッグ用情報ログtable				*/

extern UINT						f_dpdaw_exeCltCalib_flg;									/**< くクレメンテキャリブレーション実行フラグ	*/

extern UINT						f_dpdaw_regTxGainData[D_DPDA_ANTMAX][D_DPDA_CARMAX];		/**< TXゲイン設定値								*/
extern UINT						f_dpdaw_regRxGainData[D_DPDA_ANTMAX][D_DPDA_CARMAX];		/**< RXゲイン設定値								*/

extern UINT						f_dpdaw_strainParaTx_Peak[E_DPDA_STRAIN_PEAK_MAX];

extern T_DPDA_IMPULSE_COEFFICIENT	f_dpdaw_calcImpulseCoefficient[D_DPDA_IMPULSE_NUM];		/**< インパルス係数(Car1,NCO is not0)格納領域	*/


extern T_DPDA_ALM_LOG_DEBUG		*f_dpdaw_almLog;											/**< アラーム発生時解析用ログtable				*/
extern T_RRH_ALM_LOG_FOR_HIST	*f_dpdaw_almLogForHistory;									/**< アラームヒストリー用ログ情報格納位置		*/
extern T_RRH_LOG_TEMP			*f_dpdaw_tempLog;											/**< 温度ログ情報格納位置						*/

extern UINT						*f_dpdaw_spiCpldAddr;										/**< SPI CPLD系レジスタアドレステーブル			*/
extern UINT						*f_dpdaw_spiTxAddr;											/**< SPI Tx系レジスタアドレステーブル			*/
extern UINT						*f_dpdaw_spiRxAddr;											/**< SPI Rx系レジスタアドレステーブル			*/
 	
extern UINT						f_dpdaw_ant_max_num;										/**< 装置種別によるアンテナ数					*/
extern UINT						f_dpdaw_ant_unit_max_num;									/**< 装置種別によるユニット数					*/
extern UINT						f_dpdaw_carrier_max_num;									/**< 装置種別によるキャリア数(max)				*/

extern UINT						f_dpdaw_reKind;												/**< 装置種別(0:sub6 1:mmW						*/
extern UINT						f_dpdaw_ruIndex;											/**< RU Index(0:sub6 3.7G, 1:sub6 4.5G, 2:mmW 280G	*/

extern UINT						f_dpdaw_teston_flag;										/**< testtxon/offフラグ(bit毎の判定 0:OFF, 1:ON	*/
																							/**< mmw[31:16]									*/
																							/**< Ant   :              B               A		*/
																							/**< Cr	   :7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0		*/
																							/**< bit列 :0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0	    */
																							/**< sub6[15:0]									*/
																							/**< Ant   :  H   G   F   E   D   C   B   A		*/
																							/**< Cr	   :1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0		*/
																							/**< bit列 :0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0	    */
extern UINT						f_dpdaw_index_TssiPos;
extern UINT						f_dpdaw_index_AntPowPos;
extern UINT						f_dpdaw_index_RefPowPos;
extern UINT						f_dpdaw_index_RefPowPos_VSWR;
extern UINT						f_dpdaw_index_FbPowPos;
extern UINT						f_dpdaw_index_VswrPos;
extern UINT						f_dpdaw_index_RxPowPos;
extern UINT						f_dpdaw_index_RtwpPos;
extern UINT						f_dpdaw_index_CarrierPos;
extern UINT						f_dpdaw_index_AntDisPos;
extern UINT						f_dpdaw_index_A000_0200;
extern UINT						f_dpdaw_index_A000_0210;
extern UINT						f_dpdaw_index_A000_0B10;
extern UINT						f_dpdaw_index_A000_0B20;
extern UINT						f_dpdaw_index_A000_0B30;
extern UINT						f_dpdaw_index_B000_0B10;
extern UINT						f_dpdaw_index_B000_0B20;
extern UINT						f_dpdaw_index_B000_02C0;
extern UINT						f_dpdaw_index_B000_0200;
extern UINT						f_dpdaw_index_B000_0210;
extern UINT						f_dpdaw_index_SvBusPos;
extern UINT						f_dpdaw_index_B100_0040;
extern UINT						f_dpdaw_index_B100_0044;
extern UINT						f_dpdaw_index_B100_0048;
extern UINT						f_dpdaw_index_B100_004C;
extern UINT                     f_dpdaw_index_A200_0720;
extern UINT						f_dpdaw_rficSetupCmp;
extern UINT						f_dpdaw_index_A201_43C8;
extern UINT						f_dpdaw_index_B100_0050;
extern UINT						f_dpdaw_index_B100_0054;
extern UINT						f_dpdaw_index_B100_0058;
extern UINT						f_dpdaw_index_B100_005C;

extern T_DPDA_ALM_REPORT f_dpdar_alarm_report_tbl[E_DPDA_ALM_SUB6_NUM_MAX];
extern T_DPDA_ALM_REPORT f_dpdar_alarm_report_tbl_mmw[E_DPDA_ALM_MMW_NUM_MAX];

extern T_RRH_TXPOW_DETECT_TBL*		f_dpdaw_txpow_detect;
extern T_RRH_TXPOW_DETECT_LOG*		f_dpdaw_txpow_log;
extern T_RRH_LUT_LOG*				f_dpdaw_lut_log;
extern T_RRH_LOG_UPSIGSTOP_HIS*		f_dpdaw_upsigstop_log;
extern T_RRH_LOG_CAL_HIS*			f_dpdaw_cal_log;
extern struct timespec				f_dpdaw_sfnload_settime;

extern T_RRH_LOG_DLF_HIS*			f_dpdaw_dlf_log;

#endif	/* F_DPDA_RAM_LOCAL_H */
/** @} */
