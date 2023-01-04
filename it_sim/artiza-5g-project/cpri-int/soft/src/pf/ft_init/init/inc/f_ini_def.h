/*!
 * @skip  $ld:$
 * @file  f_ini_def.h
 * @brief 初期化プロセス デファイン定義.
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#ifndef F_INI_DEF_H
#define F_INI_DEF_H

/*!
 * @name タスク管理状態
 * @note タスク管理状態の定義
 * @{
 */
#define	D_INI_TSK_STN_STA					0		/**< INIT処理開始										*/
#define	D_INI_TSK_STN_INIRES_WATE			1		/**> Initialization Response待ち						*/
#define	D_INI_TSK_STN_SIG_WATE				2		/**< シグナル待ち受け状態(通常時)						*/
#define	D_INI_TSK_STN_SIG_RCV				3		/**< 送信シグナルレスポンス待ち状態						*/


/*!
 * @name 起動実施判定用データ
 * @note 起動実施判定用データの定義
 * @{
 */
#define D_INI_CREATE_PTN1					0x0001					/*!< 生成・起動パターン1				*/
#define D_INI_CREATE_PTN2					0x0002					/*!< 生成・起動パターン2(シリアル)		*/
#define D_INI_CREATE_PTN3					0x0003					/*!< 生成・起動パターン3(パラレル)		*/
#define D_INI_SKIP							0x0000					/*!< 生成・起動Skip						*/
/* @} */

/*!
 * @name 初期化プロセス処理結果
 * @note 初期化プロセス処理結果の定義
 * @{
 */
#define D_INI_OK							1						/*!< 正常終了							*/
#define D_INI_NG							0						/*!< 異常終了							*/
/* @} */

/*!
 * @name 各プロセス起動結果
 * @note 各プロセス起動結果の定義
 * @{
 */
#define D_INI_PROC_OK							1					/*!< 正常起動							*/
#define D_INI_PROC_NG							2					/*!< 異常起動							*/
#define D_INI_PROC_SKIP							3					/*!< スキップ							*/
/* @} */

/*!
 * @name シグナル応答状況
 * @note シグナル応答状況の定義
 * @{
 */
#define D_INI_SIG_RCV_WATE						0					/*!< 待機状態							*/
#define D_INI_SIG_RCV_OK						1					/*!< 受信済状態							*/
/* @} */

/*!
 * @name リセット時動作の実施有無
 * @note リセット時動作の実施有無
 * @{
 */
#define D_INI_SIG_RESET_OFF						0					/*!< リセット時動作を実施しない			*/
#define D_INI_SIG_RESET_ON						1					/*!< リセット時動作を実施する			*/
#define D_INI_SIG_RESET_CHK						3					/*!< リセット有無はALM要因に依存		*/
/* @} */

/*!
 * @name 強制リセットの実施有無
 * @note 強制リセットの実施有無
 * @{
 */
#define D_INI_COMPEL_RESET_OFF					0					/*!< 強制リセットを実施しない			*/
#define D_INI_COMPEL_RESET_ON					1					/*!< 強制リセットを実施する				*/
/* @} */

/*!
 * @name 最大プロセス数
 * @note 最大プロセス数の定義
 * @{
 */
/* 2020/12/28 M&C) Implement device startup (start modify) */
// #define D_INI_PROCNO_MAX					0x0003					/*!< 最大プロセス数(1)					*/
// 擬似環境 20210311
//#define D_INI_PROCNO_MAX					0x0009					/*!< 最大プロセス数(1)					*///C9
#define D_INI_PROCNO_MAX					0x0008					/*!< 最大プロセス数(1)					*///C9
// 擬似環境 20210311
/* 2020/12/28 M&C) Implement device startup (end modify) */
/* @} */

/*!
 * @name 起動プロセス定義
 * @note 起動プロセス定義 define
 * @{
 */
enum
{
	E_INI_STARTING_PROC_PF = 0,										/* PFプロセス							*/
	E_INI_STARTING_PROC_PTP,										/* PTPプロセス							*/
	E_INI_STARTING_PROC_INIT,										/* INITプロセス							*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_INI_STARTING_PROC_F_PF,										/* F_PFプロセス							*/
	E_INI_STARTING_PROC_L2,											/* L2プロセス							*/
	E_INI_STARTING_PROC_L3,											/* L3プロセス							*/
// 擬似環境 20210311
//	E_INI_STARTING_PROC_RE,											/* REプロセス							*/
//	E_INI_STARTING_PROC_MT											/* MTプロセス							*/
	E_INI_STARTING_PROC_MT,											/* MTプロセス							*/
	E_INI_STARTING_PROC_RE											/* REプロセス							*/
// 擬似環境 20210311
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
};
/* @} */

/*!
 * @name PTPプロセス起動定義
 * @note PTPプロセス起動定義 define
 * @{
 */
#define D_INI_PTP_MASTER_MODE				"%s -m"
#define D_INI_PTP_SLAVE_MODE				"%s -p"
/* @} */

/*!
 * @name 受信シグナル種別定義
 * @note 受信シグナル種別定義 enum
 * @{
 */
enum
{
	E_INI_SIGNAL_LOCSW = 0,											/*!< ローカルリセット[ローカルRST SW]				*/
	E_INI_SIGNAL_MP_L3_RMT,											/*!< M-plane リモートリセット			*/
	E_INI_SIGNAL_L1IND,												/*!< RE強制リセット(L1 Inband Reset)	*/
	E_INI_SIGNAL_MP_WDT,											/*!< M-plane WDTによる自律リセット		*/
	E_INI_SIGNAL_SWALM,												/*!< ソフト障害検知(ALM)				*/
	E_INI_SIGNAL_SWERR,												/*!< ソフト障害検知(ERROR)				*/
	E_INI_SIGNAL_SIGCHLD,											/*!< SIGCHLD							*/
	E_INI_SIGNAL_WDTTO,												/*!< WDT T.O							*/
	E_INI_SIGNAL_RSCALM,											/*!< リソースALM						*/
	E_INI_SIGNAL_MAX
};
/* @} */

/*!
 * @name 障害ログ要求の識別定義
 * @note 障害ログ要求の識別 enum
 * @{
 */
enum
{
	E_INI_LOGTYPE_TROUBLE = 0,										/*!< 障害ログ							*/
	E_INI_LOGTYPE_HLTCHK,											/*!< ヘルスチェック要求					*/
	E_INI_LOGTYPE_RUNNING,											/*!< 運用情報出力要求					*/
	E_INI_LOGTYPE_MAX
};
/* @} */


/*!
 * @name 障害ログ取得数
 * @note 障害ログ取得数
 * @{
 */
#define D_INI_CMD_RRHDBG_CNT				24						/*!< 障害ログ取得数(cmd_rrhdbgコマンド)	*/
/* @} */

/*
 * @name 診断用データ
 * @note
 * @{
 */
#define D_INI_BIT8							8						/*!< bit 8								*/
/* @} */

/*!
 * @name コンフィグフラグ値定義
 * @note
 * @{
 */
#define D_INI_FAULT_INIBPF					0x00000001				/* f_ini_iniBpf fault					*/
/* @} */

/*!
 * @name 共有メモリ種別数定義
 * @note MAXで128とすること。128以上は正常に動作しません
 * @{
 */
#define D_INI_CREATE_SHMCNT					44
/* @} */

/*!
 * @name シグナル応答待ち時間定義
 * @note シグナル応答待ち時間定義 define
 * @{
 */
#define D_INI_SIGWAIT_SEC					1						/* シグナル待ち時間(sec)				*/
#define D_INI_SIGWAIT_MSEC					0						/* シグナル待ち時間(msec)				*/
/* @} */

/*!
 * @name メッセージサイズ定義
 * @note INIT処理で使用するメッセージサイズの定義 define
 * @{
 */
#define D_INI_MSG_SIZE_32					32
#define D_INI_MSG_SIZE_128					128
#define D_INI_MSG_SIZE_256					256
/* @} */

/*!
 * @name リセットを実施するアラーム要因定義
 * @note リセットを実施するアラーム要因定義 define
 * @{
 */
/* リセットを実施するアラーム要因 */
#define D_INI_RSETRST_ALM					153						/* リセットを実施するアラーム要因数		*/
/* @} */

/*!
 * @name シグナルオプション情報定義
 * @note シグナルオプション情報定義 define
 * @{
 */
#define D_INI_SIGOPT_KIND					0xFFFF0000				/*!< リセット種別抽出ビット				*/
#define D_INI_SIGOPT_DPDC					0x0000000F				/*!< リセット種別抽出ビット				*/
#define D_INI_SIGOPT_ALMREN					0x0000FFFF				/*!< アラーム(エラー)要因抽出ビット		*/
/* @} */

/*!
 * @name INITプロセスプライオリティ定義
 * @note INITプロセスプライオリティ定義 define
 * @{
 */
#define D_INI_PRI_NOMAL						95						/*!< 通常プライオリティ					*/
#define D_INI_PRI_FLASHSAVE					20						/*!< FLASH保存中プライオリティ			*/

/*!
 * @name リセット割り込みマスク(MIO-32)定義
 * @note リセット割り込みマスク(MIO-32)定義  define
 * @{
 */
#define D_INI_CPLD_IRQRSTM_WDT_OFF			0xFFFFFFFE				/*!< WDT割込み解除						*/

/*!
 * @name SFP OPT制御定義
 * @note SFP OPT制御定義  define
 * @{
 */
#define D_INI_CPU_SFPCNT_CPRIEN				0x00000004				/*!< CPRI Enable						*/

/*!
 * @name System LED定義
 * @note System LED定義  define
 * @{
 */
#define D_INI_CPLD_LEDSYS_OFF				0xFFFFFCE0				/*!< LEDSYS Clear						*/

/*!
 * @name TILT出力イネーブル定義
 * @note TILT出力イネーブル定義  define
 * @{
 */
#define D_INI_CPU_TILTEN					0x00000001				/*!< TILT Enable						*/

/*!
 * @name ソフトリセット要因定義
 * @note ソフトリセット要因定義  define
 * @{
 */
#define D_INI_CPLD_RESTREQ_EXT				0x00000002				/*!< EXT								*/
#define D_INI_CPLD_RESTREQ_DPDC				0x00000001				/*!< DPDC								*/
#define D_INI_CPLD_RESTREQ_OFF				0xFFFFFFFC				/*!< EXT, DPDC OFF						*/



/*!
 * @name リセット実行待ち時間定義
 * @note リセット実行待ち時間定義  define
 * @{
 */
#define D_INI_CPLD_RWAIT_MIN				0x00000001				/*!< 最小1s								*/
#define D_INI_CPLD_RWAIT_MAX				0x0000000F				/*!< 最大15s							*/

/*!
 * @name カスケード構成時の親＝＞子リセット制御bit
 * @note カスケード構成時の親＝＞子リセット制御bit  define
 * @{
 */
#define D_INI_CSCD_RST_EXE					0x00008100				/*!< リセット要求						*/
#define D_INI_CSCD_RST_CLR					0x00000000				/*!< リセット解除							*/

/*!
 * @name 障害ログ定義
 * @note 障害ログ定義  define
 * @{
 */
#define	D_INI_COMPDATA						0x00010000				/*!< 圧縮情報サイズ						*/

#define D_INI_ACCIDENT_MIN					0						/*!< 最小0								*/
#define D_INI_ACCIDENT_MAX					2						/*!< 最大2								*/
#define D_INI_ACCIDENT_LOGSIZE				0x80000					/*!< 障害ログサイズ 512k				*/
#define D_INI_ACCIDENT_LOGADDR0				0xFCB00000				/*!< 障害ログアドレス0(0面)				*/
#define D_INI_ACCIDENT_LOGADDR1				0xFCB80000				/*!< 障害ログアドレス1(0面)				*/
#define D_INI_ACCIDENT_LOGADDR2				0xFCB00000				/*!< 障害ログアドレス2(1面)				*/
#define D_INI_HELTHCHK_LOGADDR0				0xFCB80000				/*!< ヘルスチェックログアドレス(1面)	*/


#define D_INI_FLASH_OFFSET					0x00100000				/*!< FLASH読み込みオフセット			*/
#define D_INI_FLASH_SIZE					0x00000080				/*!< FLASH読み込みサイズ				*/

/*!
 * @name Signal 数定義
 * @note Signal 数定義
 * @{
 */
#define D_INI_SIGNAL_MAX					32						/*!< Signal数定義						*/
#define D_INI_SIG1_STATUS_MAX				22
#define D_INI_SIG2_STATUS_MAX				4

/*!
 * @name Flashログ保存定義
 * @note
 * @{
 */
#define D_INI_FLASH_LOG_COMP_ON				1						/* Flashログ圧縮有り					*/
#define D_INI_FLASH_LOG_COMP_OFF			0						/* Flashログ圧縮無し					*/

/* SRE対応 - Start */
#define D_INI_DEVICE_KIND1_SRE				0x01					/* RE種別:SRE							*/
/* SRE対応 - End */

/*!
 * @name INIT_thread定義
 * @note
 * @{
 */
/* 2020/12/28 M&C) Implement device startup (start modify) */
// #define D_INI_INITHDNO_MAX					2
#define D_INI_INITHDNO_MAX					4
/* 2020/12/28 M&C) Implement device startup (end modify) */

/*!
 * @name 障害ログマトリクス定義
 * @note 障害ログマトリクス定義
 * @{
 */
#define D_LOG_EVENTNUM_ALM					1						/*!< イベント数							*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_LOG_EVENTNUM_HLT					1						/*!< イベント数							*/
#define D_LOG_EVENTNUM_RUN					1						/*!< イベント数							*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define D_INIT_STATENUM_ALM					1						/*!< 状態数								*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_INIT_STATENUM_HLT					1						/*!< 状態数								*/
#define D_INIT_STATENUM_RUN					1						/*!< 状態数								*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define D_INIT_BIT_CPRI_SFPCNT				0x00000001				/* 光モジュール出力制御 */

#define D_INIT_UP_IQEN_DLCP					0x00000010				/* IQ Disalbe処理(DLCP)					*/
#define D_INIT_UP_IQEN_ULCP					0x00000002				/* IQ Disalbe処理(ULCP)					*/
#define D_INIT_TD_TDANTEN					0x00000001				/* TDD OFF (アンテナのOFF)				*/

#define D_INIT_CA_TXANTEN_DIS				0x00000000				/* 停波(0振幅)							*/

/*!
 * @name INIT_deleteフラグ
 * @note
 * @{
 */
#define D_INI_DEL_OFF					0
#define D_INI_DEL_ON_PROC_PF			1
#define D_INI_DEL_ON_PROC_MP			2
#define D_INI_DEL_ON					3




#endif
/* @} */
