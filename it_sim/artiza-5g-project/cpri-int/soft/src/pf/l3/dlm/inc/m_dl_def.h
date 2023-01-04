/*!
 * @skip  $ld:$
 * @file  m_dl_def.h
 * @brief Constant definition.
 * @name  Constant definition.
 * @date  2008/07/30 FFCS)Wuh Create
 * @date  2008/08/05 FFCS)Wuh Define the RE file size of 3G and S3G
 * @date	  2009/01/28  FFCS)wuh M-S3G-eNBPF-01235
 * @date	  2009/02/10  FFCS)wuh 1st system parameter modification
 * @date	  2009/02/11  FFCS)wuh modify for M-S3G-eNBPF-01348
 * @date	  2009/02/23  FFCS)wuh modify for M-S3G-eNBPF-01407
 * @date      2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 * @date      2009/04/13  FFCS)Wuh modify for S3G PRC  CR-00038-005 (CPRI spec V1.07)
 * @date      2009/04/30  FFCS)Wuh modify for CR-00038-005
 *                      move the process of FLASH erasing to file data sending
 * @date      2009/07/01  FFCS)Tangj modify for CR-00055-000
 *							3G VUPU DL case, modify the DLD_RCVDAT_MKRSV_3G
 * @date  2010/01/05 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 * @date  2010/04/02 FJT)Taniguchi  modify for CR-XXXXX-XXX
 * @date  2011/09/16 COMAS)Uchida  modify for 11B-02-005：VA提案の2G/800MのSLCアーキ適用対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2011
 */

/****************************************************************************************************************************/
/* ソフトバージョン用マスク定数定義																							*/
/****************************************************************************************************************************/
/*!
 * @name define for soft version mask
 * @{
 */
#define DLD_MJRVER_MSK		0xFF00												/* メジャーバージョン用マスク				*/
#define DLD_MINVER_MSK		0x00FF												/* メジャーバージョン用マスク				*/
#define DLD_BIT_MSK         0x0001                                              /* the mask of Bit Line Rate */
/* @} */

/****************************************************************************************************************************/
/* ダウンロードサイズ定数定義																								*/
/****************************************************************************************************************************/
/*!
 * @name the size of systemparameter
 * @{
 */
/*-----TBD:the size of systemparameter is TBD-----*/
#define DLD_SYSPAR3G_SIZE   32                                                  /* the systemparameter size of 3G */
#define DLD_SYSPARS3G_SIZE  64                                                  /* the systemparameter size of S3G */
/* @} */

/****************************************************************************************************************************/
/* F社制を表すコード定義																									*/
/****************************************************************************************************************************/
/*!
 * @name maker name (3001 type)
 * @{
 */
#define DLD_MAKER1		0x0046												/* F										*/

#define DLD_MAKER2		0x0052                                              /* R                                        */


#define DLD_MJ1			0													/* maker judge 1 */
#define DLD_MJ2			1													/* maker judge 2 */

/** DCM Zynq: add new maker "K" **/
#define DLD_MAKER3		0x004B			/** K **/
#define DLD_MJ3			2

/**TDD: add new maker "V"**/
#define DLD_MAKER4		0x0056			/** V **/
#define DLD_MJ4			3

/* @} */
/****************************************************************************************************************************/
/* ソフトバージョン用マスク定数定義																							*/
/****************************************************************************************************************************/
/*!
 * @name define for soft version mask
 * @{
 */
#define DLD_BIT8			8													/* 8ビットシフト							*/
#define DLD_BIT16			16													/* 16ビットシフト							*/
/* @} */
/****************************************************************************************************************************/
/* ダウンロード管理テーブル定数定義																							*/
/****************************************************************************************************************************/
/*!
 * @name the file name number
 * @{
 */
#define DLD_FILENAME_NUM	20													/* ファイル名								*/
/* @} */
/****************************************************************************************************************************/
/* データ展開納テーブル定数定義																								*/
/****************************************************************************************************************************/

/*!
 * @name the firm file size
 * @{
 */
/* データ格納テーブル定数定義												*/
#define DLD_RCV_3G_NUM          8388608         /**< 受信データ数(REファイルチェック(from 3G)用MDEIF 8M)*/
												/**< ファイル情報報告通知受信処理で使用 */
#define DLD_RCV_S3G_NUM         16777216        /**< 受信データ数(REファイルチェック(from S3G)用MDEIF 16M)*/
												/**< ファイル情報報告通知受信処理で使用 */
#define DLD_RCVDAT_RSV			0x1CA			/**< 予約領域サイズ	458byte		*/
#define DLD_RCVDAT_MKRSV_DEBUG_3G		0x1ee   /* 494byte */
#define DLD_RCVDAT_MKRSV_3G_FILNUM3		0x1e6	/**< メーカ固有領域サイズ486byte*/
#define DLD_RCVDAT_MKRSV_3G_FILNUM4		0x1B6	/**< メーカ固有領域サイズ438byte*/
#define DLD_RCVDAT_MKRSV_S3G		0x1AE       /**< メーカ固有領域サイズ 430byte */
#define DLD_HEAD_SIZE			0x400			/**< 受信データヘッダサイズ	*/
												/**< 圧縮データサイズ			*/
#define DLD_DATA_NUM_3G			DLD_RCV_3G_NUM - DLD_HEAD_SIZE
#define DLD_DATA_NUM_S3G        DLD_RCV_S3G_NUM - DLD_HEAD_SIZE
/* @} */

/****************************************************************************************************************************/
/* FLASH領域アドレス定数定義																								*/
/****************************************************************************************************************************/

/****************************************************************************************************************************/
/* バッファ獲得リトライ回数、待ちタイマ値定数定義																			*/
/****************************************************************************************************************************/
/*!
 * @name the buffer get retry number,waiting time number
 * @{
 */
#define DLD_BUFFGET_CNT		3													/* バッファ獲得リトライ回数					*/
#define DLD_BUFFGET_WAIT100	10													/* リトライ待ちタイマ(100ms)				*/
#define DLD_BUFFGET_WAIT500	50													/* リトライ待ちタイマ(500ms)				*/
/* @} */
/****************************************************************************************************************************/
/* システムパラメータデータ数定数定義																						*/
/****************************************************************************************************************************/
/*!
 * @name the byte number of systemparameter
 * @{
 */
/*-----the size is TBD-----*/
#define DLD_SYSPRA_NUM		32													/* システムパラメータのデータ数				*/
#define DLD_SYSPRAD_NUM		16	                                            /*-----the systemparameter size for D-----*/
#define DLD_SYSPRAEF_NUM		16	                                            /*-----the systemparameter size for E and F-----*/

#define	DLD_LTE_BANDWID		17													/* LTE帯域幅								*/

/* @} */
/****************************************************************************************************************************/
/* システムパラメータ定数定義																								*/
/****************************************************************************************************************************/
/*!
 * @name the defined number of systemparameter
 * @{
 */
/* 最小値 */
#define DLD_MIN_0000		0x0000												/* シスパラの最小値(0x0000)					*/
#define DLD_MIN_0001		0x0001												/* シスパラの最小値(0x0001)					*/
#define DLD_MIN_9612		0x258C												/* 3G carrier working frequency mini number(9621) */

/* 最大値 */
#define DLD_MAX_0002		0x0002												/* シスパラの最大値(0x0002)					*/
#define DLD_MAX_0010		0x0010												/* シスパラの最大値(0x0010)					*/
#define DLD_MAX_00FF		0x00FF												/* シスパラの最大値(0x00FF)					*/
#define DLD_MAX_FFFF		0xFFFF												/* シスパラの最大値(0xFFFF)					*/
#define DLD_MAX_0001		0x0001                                              /* シスパラの最大値(0x0001)					*/
#define DLD_MAX_0004		0x0004                                              /* シスパラの最大値(0x0004)					*/
#define DLD_MAX_000F		0x000F												/* シスパラの最大値(0x000F) */
#define DLD_MAX_9888		0x26A0												/* 3G carrier working frequency max number(9621) */
#define DLD_MAX_C924		0xC924                                              /*sector chip(#2,#3,#4)/(#5,#6,#7) max number*/
#define DLD_MAX_C210		0xC210                                              /*LTE max number*/


/* Typical値 */
#define DLD_TYP_1			0x0014												/* 1 : CPRI State安定化時間(RE)				*/
#define DLD_TYP_2			0x0078												/* 2 : ヘルスチェック要求確認タイマ(RE)		*/
#define DLD_TYP_3			0x0005												/* 3 : ヘルスチェック要求未確認保護段数(RE)	*/
#define DLD_TYP_4			0x012C												/* 4 : システムパラメータ送信確認タイマ		*/
#define DLD_TYP_5			0x6D60												/* 5 : ファームウェア送信確認タイマ			*/
#define DLD_TYP_6			0x0000												/* 6 : セクタnチップ補正(0系)				*/
#define DLD_TYP_7			0x0000												/* 7 : セクタnチップ補正(1系)				*/
#define DLD_TYP_8			0x0096												/* 8 : 外部装置データ受信確認タイマ			*/
#define DLD_TYP_9			0x0096												/* 9 : FLD-MT接続確認タイマ					*/
#define DLD_TYP_10			0x0096												/* 10:FLD-MT接続解放確認タイマ				*/
#define DLD_TYP_11			0x0096												/* 11:FLD-MTデータコマンド受信確認タイマ	*/
#define DLD_TYP_12			0x0096												/* 12:FLD-MTデータファイル受信確認タイマ	*/
#define DLD_TYP_13			0x0096												/* 13:SV-MT接続確認タイマ					*/
#define DLD_TYP_14			0x0096												/* 14:SV-MT接続解放確認タイマ				*/
#define DLD_TYP_15			0x0096												/* 15:SV-MTデータ受信確認タイマ				*/
#define DLD_TYP_16			0x0002												/* 16:3G用送信ダイバーシチ設定				*/
#define DLD_TYP_17			0x0000												/* 17:RE primary system information(TBD)		*/
#define DLD_TYP_18			0x0001												/* 18:S3G bandwidth(TBD)		*/
#define DLD_TYP_19			0x0001												/* 19:3G carrier#1-4 working flag 0x00001111(TBD)		*/
#define DLD_TYP_20			0x0000												/* 20:tx system number(TBD)		*/
#define DLD_TYP_21			0x0000												/* 21:reserve for system parameter E		*/
#define DLD_TYP_22			0x0000												/* 22:reserve for system parameter E		*/
#define DLD_TYP_23			0x0000												/* 23:reserve for system parameter E		*/
#define DLD_TYP_24			0x0000												/* 24:reserve for system parameter E		*/
#define DLD_TYP_25			0x0000												/* 25:3G carrier#1 working frequency(TBD)		*/
#define DLD_TYP_26			0x0000												/* 26:3G carrier#2 working frequency(TBD)		*/
#define DLD_TYP_27			0x0000												/* 27:3G carrier#3 working frequency(TBD)		*/
#define DLD_TYP_28			0x0000												/* 28:3G carrier#4 working frequency(TBD)		*/
#define DLD_TYP_29			0x0001												/* 29:3G MT signaling/External equipment data corresponse flag(TBD)		*/
#define DLD_TYP_30			0x1770												/* 30:systemparameter timer */
#define DLD_TYP_31			0xDAC0												/* 31:ファームウェア送信確認タイマ（更新用）		*/
#define DLD_TYP_32			0x0000												/* 32:reserve for system parameter F		*/

/* 予約領域 */
#define DLD_DUMMY_0000		0x0000												/* 16:予約領域	シスパラの最小値(0x0000)	*/
#define DLD_DUMMY_FFFF		0xFFFF												/* 16:予約領域	シスパラの最大値(0xFFFF)	*/
/* @} */
/****************************************************************************************************************************/
/* ERR制御コマンド定数定義																									*/
/****************************************************************************************************************************/
/*!
 * @name EER control commond
 * @{
 */
#define DLD_ERR_INF_ON		0													/* Download File infor NG register ON			*/
#define DLD_ERR_CMP_ON		1													/* Download Complete NG register ON				*/
#define DLD_ERR_INF_OFF		2													/* Download File infor NG register OFF			*/
#define DLD_ERR_CMP_OFF		3													/* Download Complete NG register OFF				*/
#define DLD_ERR_INF_S3G_ON	4													/* Download File infor NG register ON for S3G			*/
#define DLD_ERR_CMP_S3G_ON	5													/* Download Complete NG register ON	for S3G			*/
#define DLD_ERR_INF_S3G_OFF	6													/* Download File infor NG register OFF for S3G			*/
#define DLD_ERR_CMP_S3G_OFF	7													/* Download Complete NG register OFF for S3G				*/
/* @} */

/****************************************************************************************************************************/
/* マスクパターン																											*/
/****************************************************************************************************************************/
/*!
 * @name mask pattern
 * @{
 */
#define	DLD_MSKPTN_000F			0x000F		/* マスクパターン000F			*/
#define	DLD_MSKPTN_00F0			0x00F0		/* マスクパターン00F0			*/
/* @} */
/****************************************************************************************************************************/
/* 待ちタイマ定義																											*/
/****************************************************************************************************************************/
/*!
 * @name wait time
 * @{
 */
#define DLD_WAIT_TIME		2												/* リンクERR状態変化検出用待ちタイマ(20ms)		*/
/* @} */
/*!
 * @name system enable
 * @{
 */
#define DLD_SYS_3G             0x01        /* the system enable is 3G */
#define DLD_SYS_S3G            0x02        /* the system enable is S3G */
#define DLD_SYS_CPR_BOTH           0x02        /* the system enable is both(CPRI)*/
#define DLD_SYS_RF_BOTH           0x03        /* the system enable is both(RF)*/
/* @} */
/*!
 * @name connect type
 * @{
 */
#define DLD_CONT_3G				0         /* the connection is only 3G */
/* @} */

/*!
 * @name 3G debug flag before 09A
 * @{
 */

#define DLD_SIMREP_DEBUG_FLG      CMD_OFF     /* 3G debug flag */

/* @} */
/*!
 * @name RE system set for 3G value define
 * @{
 */
#define DLD_RESYS_S3GBNDTH_NONE    0x0000    /* S3G bandwidth 0 */
#define DLD_RESYS_S3GBNDTH    0x0004    /* S3G bandwidth 20MHz */
#define DLD_RESYS_3GWKFLG	  0x000F	/* all 3G carrier working flag ON */
#define DLD_RESYS_3GWKFLG_OFF 0x0000	/* all 3G carrier working flag OFF */
/* @} */
/*!
 * @name APD reset register value define
 * @{
 */
#define DLD_REGVAL_RETON	0x0003	/*APD0/1 reset ON*/
#define DLD_REGVAL_RETOFF   0x0000	/*APD0/1 reset OFF*/
/* @} */

/*!
 * @name the carrier work flag setting 0 mask
 * @{
 */
#define DLD_MSK_FFFE	0xFFFE /* carrier1 frequecy set 0 */
#define DLD_MSK_FFFD	0xFFFD /* carrier2 frequecy set 0 */
#define DLD_MSK_FFFB	0xFFFB /* carrier3 frequecy set 0 */
#define DLD_MSK_FFF7	0xFFF7 /* carrier4 frequecy set 0 */
/* @} */

/*!
 * @name define for 3G useful carrier mask
 * @{
 */
#define DLD_MSK_0001	0x0001 /* the first bit mask */
#define DLD_MSK_0002	0x0002 /* the second bit mask */
#define DLD_MSK_0004	0x0004 /* the third bit mask */
#define DLD_MSK_0008	0x0008 /* the fourth bit mask */
#define DLD_MSK_000F	0x000F /* the all bit mask */

/* @} */
/*!
* @name define for S3G useful mask
* @{
*/
#define DLD_MSK_0007	0x0007 /* the three bit mask */
#define DLD_MSK_001F	0x001F /* the five bit mask */

#define DLD_LTE_BIT	    3 /* the three bit */
#define DLD_LTE_NUM	    5 /* the career num in parameterE's LTE*/

#define DLD_SEC_BIT	    5 /* the five bit */
#define DLD_SEC_NUM	    3 /* the sector num in parameterE's SECTOR  */

#define SYS_N_MIN	    0 /* TX system min num */
#define BRAN_N_MIN	    2 /* Branch min num*/
#define BRAN_N_MAX	    4 /* Branch max num */


/* @} */
/*!
* @name define for 3G useful carrier mask
* @{
*/
#define DLD_CAR_FLG   18 /* 3G carrier#1-4 working flag */
#define DLD_CAR1_CNT	24 /* 3G carrier#1 work frequency count number */
#define DLD_CAR2_CNT	25 /* 3G carrier#2 work frequency count number */
#define DLD_CAR3_CNT	26 /* 3G carrier#3 work frequency count number */
#define DLD_CAR4_CNT	27 /* 3G carrier#4 work frequency count number */

/* @} */
/*!
* @name define for S3G
* @{
*/
#define DLD_LTE_CNT	17 /*LTE bandwidth*/
#define DLD_SEC234_CNT	20 /* sector 2/3/4-chip branch0(0..16) */
#define DLD_SEC567_CNT	21 /* sector 5/6/7-chip branch0(0..16) */
/* @} */
/*!
 * @name define for the writing/erasing FLASH state
 * @{
 */
#define DLD_TSK_STATE_USE		0 /* use state */
#define DLD_TSK_STATE_3GWR		1 /* 3G writing state */
#define DLD_TSK_STATE_S3GWR		2 /* S3G writing state */
#define DLD_TSK_STATE_3GER		3 /* 3G erasing state */
#define DLD_TSK_STATE_S3GER		4 /* S3G erasing state */
/* @} */
/*!
 * @name define for the download type
 * @{
 */
#define DLD_TSK_TYP_NONE		0 /* no download */
#define DLD_TSK_TYP_STUP		1 /* download in startup state */
#define DLD_TSK_TYP_USE		2 /* download in USE state */
/* @} */

/*!
 * @name 3G MT 信号/外部データ対応有無の配列番号
 * @{
 */
#define DLD_3GMT_CNT			28 /* no download */
/* @} */

/** @name running history log		*/
/* @{ */
#define	DLD_RUNHIS_BODYSIZE_MAX	48		/**< ボディサイズMAX */
#define	DLD_RUNHIS_FACENUM_MAX	1024	/**< 面数MAX */
#define	DLD_RUNHIS_FLG_RCV		0x00	/**<  送受信フラグ(受信)   */
#define	DLD_RUNHIS_FLG_SND		0x01	/**<  送受信フラグ(送信)   */
/* @} */

/*!
 * @name define for the Tx Number Change
 * @{
 */
#define DLD_SV01_INS_ACT			( D_RRH_INS_BIT | D_RRH_ACT_BIT ) 	/* INS + ACT */
#define DLD_SV01_INS_ACT_ERR		( D_RRH_INS_BIT | D_RRH_ACT_BIT | D_RRH_ERR_BIT_ON )	/* INS + ACT + ERR */
#define DLD_SV89_SLP0_3				( CMD_SLP0_1_ERR_BIT | CMD_SLP2_3_ERR_BIT ) /* SLP#0~SLP#3 */
#define DLD_TXSYSNUMOFFSET			19		/* TX Number Offset From System Parameter */
/* @} */

/*FHM from here*/

/* @{ */
/*!
 * @name 実行中DL開始要因
 */
/** 未実行 */
#define DLD_FACTOR_NONE	0
/** REC */
#define DLD_FACTOR_REC	1
/** RE */
#define DLD_FACTOR_RE	2
/* @} */

/*!
* @name Layer3状態が
 * @{
*/
/* DLD_L3_STA_2  */
#define DLD_L3_STA_2	2
/* DLD_L3_STA_3  */
#define DLD_L3_STA_3	3
/* DLD_L3_STA_4  */
#define DLD_L3_STA_4	4
/* REC未接続  */
#define DLD_L3_STA_REC_IDLE		6
/* @} */

/*!
 * @name 更新開始要因
 * @{
 */
/* 未実行 */
#define DLD_UPDATE_FACTOR_NONE				0
/* RE起動(REC未接続) */
#define DLD_UPDATE_FACTOR_RESTART_REC_UNCON	1
/* RE起動(REC接続済) */
#define DLD_UPDATE_FACTOR_RESTART_REC_CON	2
/* REC主導 */
#define DLD_UPDATE_FACTOR_REC_INIT			3
/* @} */

/*!
 * @name フラッシュ書込フラグ
 * @{
 */
/** 未実行 */
#define DLD_WRITE_NONE	0
/** 書込中 */
#define DLD_WRITE_ING	1
/* @} */

/*!
 * @name DL中REファイル読込状態
 * @{
 */
/** 未実行 */
#define DLD_READ_NONE	0
/** 読込中 */
#define DLD_READ_ING	1
/** 読込済 */
#define DLD_READ_DONE	2
/** 読込中止中 */
#define DLD_READ_STOP	3
/** 読込NG */
#define DLD_READ_NG		4
/* @} */

/*!
 * @name ファームウェアファイル報告結果
 * @{
 */
/* 未実行 */
#define DLD_RESLT_IDLE						0
/* 正常 */
#define DLD_RESLT_NML							1
/* NG */
#define DLD_RESLT_NG							2
/* タイムアウト */
#define DLD_RESLT_TO							3
/* @} */

/*!
* @name 使用中タイマ(対REC情報テーブル)
 * @{
 */
/* 使用タイマなし */
#define DLD_RECRETIM_NONE						0
/* ファームウェア送信確認タイマ */
#define DLD_RECRETIM_SND						4
/*ファームウェア送信確認タイマ(更新用) */
#define DLD_RECRETIM_SND_UPDATE					14
/* @} */

/*!
 * @name 使用中タイマ
 * @{
 */
/* 使用タイマなし */
#define DLD_RERETIM_NONE						0
/* ファームウェアファイル報告確認タイマ */
#define DLD_RERETIM_REPRT						1
/* ファームウェアファイル情報報告確認タイマ */
#define DLD_RERETIM_INFREPRT					2
/* ファームウェア送信完了確認タイマ */
#define DLD_RERETIM_SNDCMP						3
/* ファームウェアファイル報告確認タイマ(更新用) */
#define DLD_RERETIM_REPRT_UPDATE 				11
/* ファームウェアファイル情報報告確認タイマ(更新用) */
#define DLD_RERETIM_INFREPRT_UPDATE				12
/* ファームウェア送信完了確認タイマ(更新用) */
#define DLD_RERETIM_SNDCMP_UPDATE				13
/* @} */

/*!
 * @name ファイル更新用 対RE状態状態一覧
 * @{
 */
/* 初期状態 */
#define DLD_RESTA_INIT							0
/* アイドル */
#define DLD_RESTA_IDLE							1
/* REファイルデータ読込中 */
#define DLD_RESTA_RE_FILEREAD					2
/* ファームウェアファイル報告応答待ち */
#define DLD_RESTA_FWREP_WAIT					3
/* ファイル情報報告応答(FW)待ち */
#define DLD_RESTA_FILEINF_WAIT					5
/* ファイルデータ送信中(FW) */
#define DLD_RESTA_FILE_TRANS					6
/* ファイルデータ送信完了報告応答(FW)待ち */
#define DLD_RESTA_FILE_COMP_WAIT				7
/* 運用中ファームウェアファイル報告応答待ち */
#define DLD_RESTA_USE_FWREP_WAIT				8
/* 運用中ファイル情報報告応答(FW)待ち */
#define DLD_RESTA_USE_FILEINF_WAIT				9
/* 運用中ファイルデータ送信中(FW) */
#define DLD_RESTA_USE_FILE_TRANS				10
/* 運用中ファイルデータ送信完了報告応答(FW)待ち */
#define DLD_RESTA_USE_FILE_COMP_WAIT			11
/* REファイル更新開始待ち */
#define DLD_RESTA_RE_UPDATE_WAIT				12
/* ファイル情報報告応答(シスパラ)待ち */
#define DLD_RESTA_FILEINF_SYSPRM_WAIT			13
/* ファイルデータ送信(シスパラ)待ち */
#define DLD_RESTA_FILE_TRANS_WAIT				14
/* ファイルデータ送信完了報告応答(シスパラ)待ち */
#define DLD_RESTA_FILE_COMP_SYSPRM_WAIT			15
/* @} */

/*!
* @name File name check return
 * @{
 */
/* File name invalid */
#define DLD_FILENAME_INVALID					0
/* ファイル名がREファイル */
#define DLD_FILENAME_RE							1
/*ファイル名がFHMファイル */
#define DLD_FILENAME_FHM						2
#define DLD_FILENAME_INTDEV						3
/* @} */


#define DLD_ECTBL_END				0xFFFFFFFF	  /*	event conversion table end	*/
#define EC_DLM_FHMDL_EVT_MAX		18
#define EC_DLM_FHMDL_STN_MAX		8
#define EC_DLM_RECDL_EVT_MAX		24
#define EC_DLM_RECDL_STN_MAX		12
#define DLD_ALL_RE					0xFF

/****************************************************************************/
/*!
 * @brief  Main event
 */
typedef enum {
	EC_DLM_EVT_INIREQ						, /* 0: スレッド起動 */
	EC_DLM_EVT_RESYSPARA_UPDATE_REQ         , /* 1: REシステムパラメータ更新要求 */
	EC_DLM_EVT_REFILE_UPDATE_REQ            , /* 2: REファイル更新要求 */
	EC_DLM_EVT_REFILE_UPDATE_STOP_REQ       , /* 3: REファイル更新中止要求 */
	EC_DLM_EVT_REFILE_UPDATE_STOP_IND       , /* 4: REファイル更新中止指示 */
	EC_DLM_EVT_USEREFILE_UPDATE_START_RSP   , /* 5: 運用中REファイル更新開始応答 */
	EC_DLM_EVT_LOADFIRM_RSP                 , /* 6: REファイルデータ読込応答 */
	EC_DLM_EVT_CANCEL_LOADFIRM_RSP          , /* 7: REファイルデータ読込中止応答 */
	EC_DLM_EVT_SAVEFIRM_RSP                 , /* 8: FWファイル更新応答 */
	EC_DLM_EVT_CPRI_FILEINFREQ              , /* 9: ファイル情報報告通知 */
	EC_DLM_EVT_CPRI_FILEINFRES              , /* 10: ファイル情報報告応答 */
	EC_DLM_EVT_CPRI_FILEDATSND              , /* 11: ファイルデータ送信 */
	EC_DLM_EVT_CPRI_FILSNDFINNTC            , /* 12: ファイルデータ送信完了報告通知 */
	EC_DLM_EVT_CPRI_FILSNDFINRES            , /* 13: ファイルデータ送信完了報告応答 */
	EC_DLM_EVT_CPRI_FIRMFILEREQ             , /* 14: ファームウェアファイル報告要求 */
	EC_DLM_EVT_CPRI_FIRMFILERES             , /* 15: ファームウェアファイル報告応答 */
	EC_DLM_EVT_CPRI_UFILEINFREQ             , /* 16: 運用中ファイル情報報告通知 */
	EC_DLM_EVT_CPRI_UFILEINFRES             , /* 17: 運用中ファイル情報報告応答 */
	EC_DLM_EVT_CPRI_UFILEDATSND             , /* 18: 運用中ファイルデータ送信 */
	EC_DLM_EVT_CPRI_UFILSNDFINNTC           , /* 19: 運用中ファイルデータ送信完了報告通知 */
	EC_DLM_EVT_CPRI_UFILSNDFINRES           , /* 20: 運用中ファイルデータ送信完了報告応答 */
	EC_DLM_EVT_CPRI_UFIRMFILEREQ            , /* 21: 運用中ファームウェアファイル報告要求 */
	EC_DLM_EVT_CPRI_UFIRMFILERES            , /* 22: 運用中ファームウェアファイル報告応答 */
	EC_DLM_EVT_TO_FILEDATSND                , /* 23: ファイルデータ送信間隔タイマ */
	EC_DLM_EVT_TO_SYSPRM_INF                , /* 24: システムパラメータ情報報告確認タイマ */
	EC_DLM_EVT_TO_SYSPRM_SNDFIN             , /* 25: システムパラメータ送信完了確認タイマ */
	EC_DLM_EVT_TO_SYSPRMSND_S3G             , /* 26: システムパラメータ送信確認タイマ */
	EC_DLM_EVT_TO_FIRMFILE                  , /* 27: ファームウェアファイル報告確認タイマ */
	EC_DLM_EVT_TO_FIRMFILE_INF              , /* 28: ファームウェアファイル情報報告確認タイマ */
	EC_DLM_EVT_TO_FIRMFILE_SNDFIN           , /* 29: ファームウェア送信完了確認タイマ */
	EC_DLM_EVT_TO_FIRMWARESND_S3G           , /* 30: ファームウェア送信確認タイマ */
	EC_DLM_EVT_TO_UFIRMFILE                 , /* 31: ファームウェアファイル報告確認タイマ(更新用) */
	EC_DLM_EVT_TO_UFIRMFILE_INF             , /* 32: ファームウェアファイル情報報告確認タイマ(更新用) */
	EC_DLM_EVT_TO_UFIRMFILE_SNDFIN          , /* 33: ファームウェア送信完了確認タイマ(更新用) */
	EC_DLM_EVT_TO_USEFIRMWARESND            , /* 34: ファームウェア送信確認タイマ(更新用) */
	EC_DLM_EVT_TO_REFILE_ERASE_RSP,
	EC_DLM_MAIN_EVT_END,
	EC_DLM_MAIN_EVT_MAX
} EC_DLM_MAIN_EVT;

/****************************************************************************/
/*!
 * @brief  対REC状態
 */
typedef enum {
    EC_DLM_MAIN_STN_INIT                                , /* 0: 初期状態 */
    EC_DLM_MAIN_STN_IDLE                                , /* 1: アイドル */
    EC_DLM_MAIN_STN_FHMDL                               , /* 2: FHM自律DL処理中 */
    EC_DLM_MAIN_STN_FILEINF_REP_SYSPARA_CON             , /* 3: ファイル情報報告応答(シスパラ)集約中 */
    EC_DLM_MAIN_STN_FILEDATATRANS_SYSPARA_WAIT          , /* 4: ファイルデータ送信(シスパラ)待ち */
    EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_SYSPARA_CON    , /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
    EC_DLM_MAIN_STN_FW_REP_CON                          , /* 6: ファームウェア報告応答集約中 */
    EC_DLM_MAIN_STN_FILEDATATRANS_DES_WAIT              , /* 7: ファイルデータ転送先決定待ち */
    EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT               , /* 8: ファイルデータ送信(FW)待ち */
    EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_WAIT           , /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
    EC_DLM_MAIN_STN_USE_FW_REP_CON                      , /* 10: 運用中ファームウェア報告応答集約中 */
    EC_DLM_MAIN_STN_USE_FILEDATATRANS_DES_WAIT          , /* 11: 運用中ファイルデータ転送先決定待ち */
    EC_DLM_MAIN_STN_USE_FILEDATATRANS_FW_WAIT           , /* 12: 運用中ファイルデータ送信(FW)待ち */
    EC_DLM_MAIN_STN_USE_FILEDATATRANS_FINREP_WAIT       , /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
    EC_DLM_MAIN_STN_MAX                                   /* Max */
} EC_DLM_MAIN_STN;

/****************************************************************************************************************************/
/* 再送回数																													*/
/****************************************************************************************************************************/
/*!
 * @name Max retry number
 * @{
 */
/* ファイル情報報告(システムパラメータ)通知再送回数 */
#define DLD_FILEINFREP_SYSPARA_MAX				3
/* システムパラメータデータ送信再送回数 */
#define DLD_FILEDATATRANS_SYSPARA_MAX			3

#define DLD_FILEINFREP_MAX 3

/* --resend counter type */
/*再送回数[ファームウェアファイル報告要求]	*/
#define		DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ						0
/*再送回数[ファイル情報報告通知]			*/
#define		DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE					1
/*再送回数[ファイルデータ送信完了報告通知]	*/
#define		DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE	2
/* counter max */
#define		DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ_MAX						3
#define		DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE_MAX					3
#define		DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE_MAX	3

/*!
 * @name download memory handle
 */
/* @{ */

#define dlw_handle gt_dlw_mnt_reinf[CMD_NUM0].file_addr

/* @} */
