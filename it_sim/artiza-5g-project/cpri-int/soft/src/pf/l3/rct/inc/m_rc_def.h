/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_rc_def.h
 *  @brief	RE control task Common Definition
 *  @date   2008/07/29 FFCS)linlj create
 *  @date   2010/02/19 QNET)Kabasima CR-xxxxx-xxx [DCM向け]TS-145-ALM制御時のRE動作の件
 *  @date   2010/02/19 QNET)Kabasima M-S3G-eNBSYS-01090:☆ST_ENB☆TS232項：機能部のUSE bit ON条件について
 *  @date   2010/03/16 QNET)Kabasima CR-xxxxx-xxx:[DCM向け]REハードソフトインタフェース仕様書v2.xx版リリース
 *  @date   2011/03/10 FJT)Koshida M-S3G-eNBSYS-XXXXX:TS137対応(リソースALM時の故障状態/基本機能監視項目見直し)
 *  @date   2011/05/13 FJT)koshida modify for CeNB-F-073-008(F)(REにおける故障発生時の機能部状態報告)
 *  @date   2011/09/17 FJT)Tokunaga modify for M-S3G-eNBSYS-02497 TS099項対応
 *  @date   2011/09/18 COMAS)Uchida  modify for 11B-01-004:RE装置構成情報の項目「対応周波数帯」の予約bitの値について
 *  @date   2011/10/10 FFCS)Linlj  modify for 省電力対応
 *  @date   2011/11/24 COMAS)Uchida modify for M-S3G-eNBSYS-02539対応
 *  @date   2013/02/18 CMS)Andou XXXXXXXX TRX設定応答のコード変更
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 *  @date   2016/11/16 FJT)ohashi 16B スロット数制限機能追加
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*****************************************************************************/

#ifndef M_RC_DEF_HEAD
#define M_RC_DEF_HEAD
/*--------------------------------------------------------------------------*/
/* m_rc_defヘッダの基本構成													*/
/*	No.0：RE制御タスク共通関数関連定義(パラメータ)							*/
/*	No.1：RE制御タスク個別関数関連定義(パラメータ)							*/
/*	No.2：テーブル読出し用デファイン定義									*/
/*	No.3：テーブル読出し用マスク定義										*/
/*	No.4：汎用デファイン定義												*/
/*--------------------------------------------------------------------------*/

/****************************************************************************/
/*	- No.0 -																*/
/*						RE制御タスク共通関数関連							*/
/*																			*/
/****************************************************************************/
/****************************************************************************/
/* m_rc_cm_TraRstExec関連定義	(自カードリセット実行関数)					*/
/****************************************************************************/
/* INパラメータ：リセット種別		*/
#define	RCD_LCL_RST				0		/**<  ローカルリセット					*/
#define	RCD_RMT_RST_COM			1		/**<  リモートリセット(共通部含)		*/
#define	RCD_RMT_RST_NOCOM		2		/**<  リモートリセット(共通部含まず)	*/
#define	RCD_RMT_RST_RE_ALL		3		/**<  リモートリセット(RE全体)			*/
#define	RCD_CPRI_DL_RST			4		/**<  ダウンロード後の自律リセット(共通部含)*/

/****************************************************************************/
/* m_rc_cm_RstAlmExte関連定義	(RST_ALM延長関数)							*/
/****************************************************************************/
/* INパラメータ：制御種別			*/
#define	RCD_LCL_RST				0		/**<  ローカルリセット					*/
#define	RCD_LCL_ALM				1		/**<  ローカルALM						*/
/* INパラメータ：制御タイミング		*/
#define	RCD_EXTE				0		/**<  延長								*/
#define	RCD_DRCT 				1		/**<  即時								*/


/****************************************************************************/
/* m_rc_cm_TraSetPosJug関連定義	(TRA設定可能判定関数)						*/
/****************************************************************************/
/* INパラメータ：USEチェック		*/
#define	RCD_CHK_UNNCY			0		/**<  USEチェック不要					*/
#define	RCD_CHK_NCY				1		/**<  USEチェック要					*/
										/* (補足:NCY->necessary)			*/
#define	RCD_CHK_ARRGNUM			2		/**<  USEチェック格納配列数			*/

/* INパラメータ：USEチェック		*/
#define	RCD_CARD_STA_ARRGNUM	10		/**<  カード状態格納配列数				*/

/* RETURN値：チェック結果			*/
#define	RCD_TRA0_NML			0		/**<  TRA0のみ正常						*/
#define	RCD_TRA1_NML			1		/**<  TRA1のみ正常						*/
#define	RCD_TRA_BTH_NML			2		/**<  両系TRA正常						*/
#define	RCD_TRA_BTH_ABNML		3		/**<  両系TRA異常						*/
#define	RCD_OFTRA_NML			4		/**<  OFTRA正常						*/
#define	RCD_OFTRA_ABNML			5		/**<  OFTRA異常						*/
#define	RCD_OFRCD_TRA0_NML		6		/**<  OFTRA0のみ正常					*/
#define	RCD_OFTRA1_NML			7		/**<  OFTRA1のみ正常					*/
#define	RCD_OFTRA_BTH_NML		8		/**<  両系OFTRA正常					*/
#define	RCD_OFTRA_BTH_ABNML		9		/**<  両系OFTRA異常					*/
#define	RCD_TRA_STATE_MAX		10		/**<  配列定義用最大値					*/
										/* (補足:NML->nomality  )			*/
										/* (    :BTH->both      )			*/

/****************************************************************************/
/* m_rc_cm_2TraChk関連定義		(2枚実装TRAチェック)						*/
/* m_rc_cm_1TraChk関連定義		(1枚実装TRAチェック)						*/
/****************************************************************************/
/* INパラメータ：カード状態チェック結果の配列 */
/*#define	RCD_CHKRSLT_TRA0ACT		0*/		/**<  TRA0実装チェック結果格納位置		*/
/*#define	RCD_CHKRSLT_TRA1ACT		1*/		/**<  TRA1実装チェック結果格納位置		*/
#define	RCD_CHKRSLT_TRA0ALM		0		/**<  TRA0ALMチェック結果格納位置		*/
#define	RCD_CHKRSLT_TRA1ALM		1		/**<  TRA1ALMチェック結果格納位置		*/
#define	RCD_CHKRSLT_TRA0USE		2		/**<  TRA0USEチェック結果格納位置		*/
#define	RCD_CHKRSLT_TRA1USE		3		/**<  TRA1USEチェック結果格納位置		*/

#define	RCD_CHKRSLT_ARRGNUM		6		/**<  TRA状態チェック格納配列数		*/


/****************************************************************************/
/* m_rc_cm_FrqCarSet関連定義	(周波数キャリア設定関数)					*/
/* m_rc_TrxAutoCarSet関連定義	(自律キャリアON設定処理)					*/
/* m_rc_cm_TrxRelExec関連定義	(TRX解放実施処理)							*/
/****************************************************************************/
/* INパラメータ：TRA種別			*/
#define	RCD_TRA_BR0				0		/**<  TRA Br0							*/
#define	RCD_TRA_BR1				1		/**<  TRA Br1							*/
/* TD-SRE TRX setting S. */
#define	RCD_TRA_BR2				2		/**<  TRA Br2							*/
#define	RCD_TRA_BR3				3		/**<  TRA Br3							*/
#define RCD_TRA_BR_NT2R			0x0003  /**<  TRA BR#0 BR#1					*/
#define RCD_TRA_BR_NT4R			0x000f  /**<  TRA BR#0 BR#1  BR#2  BR3		       */
/* TD-SRE TRX setting E. */

/****************************************************************************/
/* m_rc_TrxRelNtcRcv関連定義	(TRX解放通知受信処理)					*/
/****************************************************************************/
/* INパラメータ：TRA(Br0,Br1)状態変化	*/
#define	RCD_TRA_STACHG_ARRGNUM	2		/**<  TRA(Br0,Br1)状態変化配列			*/

/****************************************************************************/
/* m_rc_GetName関連定義		(カード名取得関数)								*/
/****************************************************************************/
/* INパラメータ：獲得指定			*/
#define	RCD_CARDNAME			0		/**<  カード名指定						*/
#define	RCD_NAME				1		/**<  名称指定							*/

/* 内部処理用						*/
#define	RCD_MAX_CARDNAME		8		/**<  カード名文字数					*/
#define	RCD_MAX_NAME			10		/**<  名称最大文字数					*/
#define	RCD_CARDNAME_MAX		6		/**<  カード品名最大文字数				*/
							/* カード名称変換テーブルカード情報最大数		*/
#define	RCD_CINF_TBL_MAX		10
							/**< カード名称変換テーブルバージョン報告最大数	*/
#define	RCD_VER_TBL_MAX			32

/****************************************************************************/
/*	- No.1 -																*/
/*						RE制御タスク個別関数関連							*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/* m_rc_ReCardCntExec関連定義	(REカード制御実施関数)						*/
/****************************************************************************/
/* Return値							*/
#define	RCD_MDIF_RST			1		/**<  自カードリセット					*/


/****************************************************************************/
/* m_rc_ReCarStaRep				(REキャリア状態報告関数)					*/
/****************************************************************************/
/* キャリアALM/ERRのONOFF状態		*/
#define	RCD_CARALM_OFF			0		/**<  キャリアALMオフ					*/
#define	RCD_CARALM_ON			1		/**<  キャリアALMオン					*/
#define	RCD_CARERR_OFF			0		/**<  キャリアERRオフ					*/
#define	RCD_CARERR_ON			1		/**<  キャリアERRオン					*/
#define	RCD_CARUSE_OFF			0		/**<  キャリアUSEオフ					*/
#define	RCD_CARUSE_ON			1		/**<  キャリアUSEオン					*/


/* スロット補正値					*/
#define	RCD_SLOTRVI_OFF			0		/**<  スロット番号補正値なし			*/

/* TRA状態							*/
#define	RCD_TRASTA_0			0		/**<  TRA状態0							*/
#define	RCD_TRASTA_1			1		/**<  TRA状態1							*/
#define	RCD_TRASTA_2			2		/**<  TRA状態2							*/
#define	RCD_TRASTA_3			3		/**<  TRA状態3							*/
#define	RCD_TRASTA_4			4		/**<  TRA状態4							*/
#define	RCD_TRASTA_5			5		/**<  TRA状態5							*/
#define	RCD_TRASTA_6			6		/**<  TRA状態6							*/
#define	RCD_TRASTA_7			7		/**<  TRA状態7							*/
#define	RCD_TRASTA_8			8		/**<  TRA状態8							*/
#define	RCD_TRASTA_9			9		/**<  TRA状態9							*/

/* キャリアALM情報					*/

#define	RCD_CARALM_221			221		/**<  両ブランチ故障 221		*/
#define	RCD_CARALM_222			222		/**<  TRX-INF ALM  222		*/
#define	RCD_CARALM_223			223		/**<  BLK状態        223		 */
#define	RCD_CARALM_224			224		/**<  3GからのREカード制御によるERR状態 224		*/
#define RCD_CARALM_CONV(almcd)	(almcd&0xFF)	/**<  carrier resource ALM Code convert */
#define	RCD_CARALM_255			255		/**<  スリープ状態   255		*/
#define RCD_CARALM_CD_NUM		256		/**< 221-255 used				*/

/* キャリアERR情報					*/

#if 0
#define	RCD_CARERR_221			221		/**<  キャリアERR TRA片系未実装		*/
#define	RCD_CARERR_222			222		/**<  キャリアERR TRA片系ALM			*/
#define	RCD_CARERR_223			223		/**<  キャリアERR TRA片系閉塞			*/
#define	RCD_CARERR_224			224		/**<  キャリアERR TRA片系ERR			*/
#define	RCD_CARERR_225			225		/**<  キャリアERR MDIF高速IF同期NG		*/
#endif

#define RCD_CARERR_224			224		/**<  APD#0ウオツチドツグタイム異常、起動異常等      */
#define RCD_CARERR_225			225		/**<  APD#0SPIバスビジタイムアウト                   */
#define RCD_CARERR_226			226		/**<  APD#0 FW-PLLアラーム検出                       */
#define RCD_CARERR_227			227		/**<  APD#0 FB-PLLアラーム検出                       */
#define RCD_CARERR_228			228		/**<  APD#0 RX-PLLアラーム検出                       */
#define RCD_CARERR_232			232		/**<  APD#0高温検出                                  */
#define RCD_CARERR_233			233		/**<  APD#0歪補償制御異常                            */
#define RCD_CARERR_234			234		/**<  APD#0 送信利得異常                             */
#define RCD_CARERR_235			235		/**<  APD#0過出力検出                                */
#define RCD_CARERR_236			236		/**<  APD#0 受信利得異常                             */
#define RCD_CARERR_238			238		/**<  APD#0 受信利得異常                             */
#define RCD_CARERR_240			240		/**<  APD#1ウオツチドツグタイム異常、起動異常等      */
#define RCD_CARERR_241			241		/**<  APD#1SPIバスビジタイムアウト                   */
#define RCD_CARERR_242			242		/**<  APD#1 FW-PLLアラーム検出                       */
#define RCD_CARERR_243			243		/**<  APD#1 FB-PLLアラーム検出                       */
#define RCD_CARERR_244			244		/**<  APD#1 RX-PLLアラーム検出                       */
#define RCD_CARERR_248			248		/**<  APD#1高温検出                                  */
#define RCD_CARERR_249			249		/**<  APD#1歪補償制御異常                            */
#define RCD_CARERR_250			250		/**<  APD#1 送信利得異常                             */
#define RCD_CARERR_251			251		/**<  APD#1過出力検出                                */
#define RCD_CARERR_252			252		/**<  APD#1 受信利得異常                             */
#define RCD_CARERR_254			254		/**<  APD#1 受信利得異常                             */


/* TRA0キャリア状態配列数				*/
#define	RCD_TRA0STA_ARRGNUM		10		/**<  TAR0キャリア状態配列数			*/

/*  TRA1キャリア状態配列数				*/
#define	RCD_TRA1STA_ARRGNUM		8		/**< TAR1キャリア状態配列数			*/

/* スロット配列数					*/
#define	RCD_SLOT_ARRGNUM		2		/**< スロット配列数					*/

/* DIV配列数					*/
#define	RCD_DIV_ARRGNUM			9		/**<  DIV有りの配列数					*/

/* 高速IF同期NG */
#define RCD_HISPEEDIF_OK		0		/**<  MDIF高速IF同期NG発生なし			*/
#define RCD_HISPEEDIF_NG		1		/**<  MDIF高速IF同期NG発生あり			*/

#define RCD_HISPDIF_NG_BR0		1		/**<  MDIF高速IF同期NG発生(Br0)		*/
#define RCD_HISPDIF_NG_BR1		2		/**<  MDIF高速IF同期NG発生(Br1)		*/
#define RCD_HISPDIF_NG_BTH		3		/**<  MDIF高速IF同期NG発生(BTH)		*/


/* TRA状態 */
#define RCD_TRA_NOINS			0		/**<  TRA 未実装状態					*/
#define RCD_TRA_ACT				1		/**<  TRA 運用中状態					*/
#define RCD_TRA_ALM				2		/**<  TRA ALM状態						*/
#define RCD_TRA_BLK				3		/**<  TRA BLK状態						*/
#define RCD_TRA_ERR				4		/**<  TRA ERR状態						*/
#define RCD_TRA_HIIF			5		/**<  TRA 高速IF同期NG状態				*/
#define RCD_TRA_INS				6		/**<  TRA INS							*/
#define RCD_TRASTA_ARRGNUM		7		/**<  TRA状態配列数					*/

/* BR状態 */
#define RCD_BR_USE			0		/**<  BR USE状態					*/
#define RCD_BR_ALM				1		/**<  BR ALM状態						*/
/* #define RCD_BR_ERR				2	*/	/**<  BR ERR状態-PA-OFF			*/
#define RCD_BRSTA_ARRGNUM		2		/**<  BR状態配列数					*/

/****************************************************************************/
/* m_rc_TraCarStaNoGet			(TRAキャリア状態番号取得関数)				*/
/****************************************************************************/
/* Return値							*/
#define	RCD_STANO_ACTUSE		0		/**<  ACT+USEの状態番号				*/
#define	RCD_STANO_ACT			1		/**<  ACTの状態番号					*/
#define	RCD_STANO_ACTUSEERR		2		/**<  ACT+USE+ERRの状態番号			*/
#define	RCD_STANO_ACTERR		3		/**<  ACT+ERRの状態番号				*/
#define	RCD_STANO_ALM			4		/**<  ALMの状態番号					*/
#define	RCD_STANO_BLK			5		/**<  BLKの状態番号					*/
#define	RCD_STANO_ERRBLK		6		/**<  ERR+BLKの状態番号				*/
#define	RCD_STANO_NOINS			7		/**<  未実装の状態番号					*/

/****************************************************************************/
/* m_rc_ReSlotRep				(REスロット情報報告関数)					*/
/****************************************************************************/
/* MDIF機能部情報					*/
#define	RCD_AMP_M_FNCINF		0x0001	/**<  AMP時 MDIF機能部情報				*/
#define	RCD_OFNODIV_M_FNCINF	0x007D	/**<  OFDivなし時 MDIF機能部情報		*/
#define	RCD_OFDIV_M_FNCINF		0x007D	/**<  OFDivあり時 MDIF機能部情報		*/

/* EXT機能部情報					*/
#define	RCD_AMP_E_FNCINF		0x007E	/**<  AMP時 EXT機能部情報				*/
/* TRA0機能部情報					*/
#define	RCD_AMP_T0_FNCINF		0x0001	/**<  AMP時 TRA0機能部情報				*/
#define	RCD_OF_T0_FNCINF		0x0001	/**<  OF時 TRA0機能部情報				*/
/* TRA1機能部情報					*/
#define	RCD_AMP_T1_FNCINF		0x0001	/**<  AMP時 TRA1機能部情報				*/
#define	RCD_OF_T1_FNCINF		0x0001	/**<  OF時 TRA1機能部情報				*/
/* TRA機能部情報					*/
#define	RCD_OFNODIV_T_FNCINF	0x0001	/**<  OFDivなし時 TRA機能部情報		*/

/****************************************************************************/
/* m_rc_ReVerRep				(REバージョン報告関数)						*/
/****************************************************************************/
/* 応答パラメータ					*/
#define	RCD_VERREP_INFNUM		1		/**<  情報数1							*/

/****************************************************************************/
/* m_rc_ReCardStaRep			(REカード状態報告関数)						*/
/****************************************************************************/
/* TRA読出しテーブルフラグ */
#define	RCD_Br0_TBL				0		/**<  読出しテーブル(Br0)				*/
#define	RCD_Br1_TBL				1		/**<  読出しテーブル(Br1)				*/

/****************************************************************************/
/*	- No.2 -																*/
/*						テーブル読出し用デファイン							*/
/*																			*/
/****************************************************************************/
/* ■MDIF SV状態管理テーブル	*/
#define	RCD_MAX_ERR_TTRY		2		/**<  ERR情報の配列数					*/
/* ■TRA SV状態管理テーブル	*/
#define	RCD_MAX_TRA_ERR_TTRY	4		/**<  ERR情報の配列数					*/
/* ■EXT SV状態管理			*/
#define	RCD_MAX_EXT_ERR_TTRY	2		/**<  ERR情報の配列数					*/

/* ■RE装置構成情報テーブル */
/* 対応周波数帯 */
#define	RCD_DIVTYP_DIV			1		/**<  Divあり							*/
#define	RCD_DIVTYP_NODIV		2		/**<  Divなし							*/
#define	RCD_DIVTYP_NODIV4		4		/**<  Divなし							*/


/* ■TRA SV状態管理テーブル */
#define	RCD_TRA_AMPDWN_3G			0x8000	/**<  AMP部電源断	3G					*/
#define	RCD_TRA_AMPDWN_S3G		0x0004	/**<  AMP部電源断	S3G					*/
#define	RCD_TRA_WARMUP			0x0002	/**<  WarmUp							*/
#define	RCD_TRA_SLEEP			0x0040	/**<  スリープ							*/

/* ■インベントリテーブル	*/
#define	RCD_CARDATCLNAME_MAXARY	12		/**<  カード品名最大配列数				*/

/* ■カード制御テーブル		*/
#define RCD_CARDCNTTBL_DATANUM	4		/**<  テーブルデータ数					*/

/****************************************************************************/
/*	- No.3 -																*/
/*						テーブル読出し用マスク定義							*/
/*																			*/
/****************************************************************************/
/* 各カードのテーブル読出し用マスク定義	*/
/* m_cm_GetCardStaで使用 */

/***************************/
/* MIDF SV状態管理テーブル */
/***************************/
#define	RCD_MDIF_REG_RST		0x0020	/**<  リセット制御						*/
#define	RCD_MDIF_REG_INS		0x0080	/**<  実装								*/
#define	RCD_MDIF_REG_NOINS		0x0040	/**<  未実装							*/
#define	RCD_MDIF_REG_ACT		0x0020	/**<  USE								*/
#define	RCD_MDIF_REG_ALM		0x0002	/**<  ALM								*/
#define	RCD_MDIF_REG_ERR		0x0001	/**<  ERR								*/
#define	RCD_MDIF_REG_BLK		0x0004	/**<  BLK								*/
#define	RCD_MDIF_REG_TST		0x0002	/**<  診断								*/

#define	RCD_MDIF_ALLBITON		0x03A6	/**<  全BitON							*/


/**************************/
/* TRA SV状態管理テーブル */
/**************************/
#define	RCD_TRA_REG_LCL			0x0080	/**<  ローカル制御						*/
#define	RCD_TRA_REG_RMT			0x0040	/**<  リモート制御						*/
#define	RCD_TRA_REG_RST			0x0020	/**<  リセット制御						*/
#define	RCD_TRA_REG_USE			0x0004	/**<  USE								*/
#define	RCD_TRA_REG_ALM			0x0002	/**<  ALM								*/
#define	RCD_TRA_REG_ERR			0x0001	/**<  ERR								*/

#define	RCD_TRA_REG_INS			0x0080	/**<  実装								*/
#define	RCD_TRA_REG_NOINS		0x0040	/**<  未実装							*/
#define	RCD_TRA_REG_ACT			0x0020	/**<  ACT								*/
#define	RCD_TRA_REG_BLK			0x0004	/**<  BLK								*/
#define	RCD_TRA_REG_TST			0x0002	/**<  診断								*/
#define	RCD_TRA_REG_AMPDWN		0x0080	/**<  電源断						*/
#define	RCD_TRA_REG_TRAON		0x0040	/**<  起動								*/
#define	RCD_TRA_REG_WARMUP		0x0002	/**<  WarmUp							*/

#define	RCD_TRA_ALLBITON		0x07A7	/**<  全BitON							*/

/**************************/
/* EXT SV状態管理テーブル */
/**************************/
#define	RCD_EXT_REG_LCL			0x0080	/**<  ローカル制御						*/
#define	RCD_EXT_REG_RMT			0x0040	/**<  リモート制御						*/
#define	RCD_EXT_REG_RST			0x0020	/**<  リセット制御						*/
#define	RCD_EXT_REG_ALM			0x0002	/**<  ALM								*/
#define	RCD_EXT_REG_ERR			0x0001	/**<  ERR								*/

#define	RCD_EXT_REG_INS			0x0080	/**<  実装								*/
#define	RCD_EXT_REG_NOINS		0x0040	/**<  未実装							*/
#define	RCD_EXT_REG_ACT			0x0020	/**<  運用								*/
#define	RCD_EXT_REG_BLK			0x0004	/**<  閉塞								*/

#define	RCD_EXT_ALLBITON		0x03A4	/**<  全BitON							*/

/************************/
/* 装置構成情報テーブル */
/************************/

/* RE種別		*/
#define	RCD_ETYP_MSK			0x0001	/**<  RE種別マスク						*/
/* 装置構成情報 */
#define	RCD_DIVTYP_MSK			0x0001	/**<  装置号番マスク					*/
/* フィルタタイプ */
#define	RCD_FIITYPE_MSK			0x0003	/**<  フィルタタイプマスク				*/

/* 800MHz帯フィルタタイプ */
#define	RCD_FIITYP_A			0x0000	/**<  フィルタタイプA					*/
#define	RCD_FIITYP_B			0x0001	/**<  フィルタタイプB					*/

/* 装置タイプ関連レジスタ */
#define	RCD_EQU_TYP_11			0x0001	/**<  RESERV  DIV無し  DC電源       OF	*/
#define	RCD_EQU_TYP_17			0x0007	/**<  RESERV  DIV無し  100_200共用  OF	*/
#define	RCD_EQU_TYP_19			0x0009	/**<  RESERV  DIV有り  DC電源       OF	*/
#define	RCD_EQU_TYP_1F			0x000F	/**<  RESERV  DIV有り  100_200共用  OF	*/
#define	RCD_EQU_TYP_ELSE		0xFFFF	/**<  該当無し							*/

/* 架の品名変換用800MHz帯フィルタタイプ */
#define	RCD_FIL_TYP_A_00		0x0000	/**<  フィルタタイプA					*/
#define	RCD_FIL_TYP_B_01		0x0001	/**<  フィルタタイプB					*/
#define	RCD_FIL_TYP_ELSE		0xFFFF	/**<  フィルタタイプELSE				*/

/* 装置名称識別 */
#define	RCD_EQU_CARD			0x0000	/**<  装置名称識別がカードの場合		*/
#define	RCD_EQU_2G				0x0001	/**<  装置名称識別が2G-OFの場合		*/
#define	RCD_EQU_17G				0x0002	/**<  装置名称識別が17G-OFの場合		*/
#define	RCD_EQU_800M			0x0003	/**<  装置名称識別が800M-OFの場合		*/
#define	RCD_EQU_15G				0x0004	/**<  装置名称識別が15G-OFの場合		*/
#define	RCD_EQU_ELSE			0x0005	/**<  装置名称識別が該当しない場合		*/

/* パラークラスインベントリ情報 */
#define	RCD_PWR_CLASS_INV_10	0x0001	/**<  パワークラス10W					*/
#define	RCD_PWR_CLASS_INV_5		0x0002	/**<  パワークラス5W					*/
#define	RCD_PWR_CLASS_INV_125	0x0003	/**<  パワークラス0.125mw				*/

/* パワークラス読み替え値 */
#define	RCD_PWR_CLASS_10		0x000A	/**<  パワークラス報告値10W			*/
#define	RCD_PWR_CLASS_40		0x0028	/**<  パワークラス報告値40W			*/
#define	RCD_PWR_CLASS_20		0x0014	/**<  パワークラス報告値20W			*/
#define	RCD_PWR_CLASS_500		0x01F4	/**<  パワークラス報告値500uW			*/

/* パワークラス単位 */
#define RCD_PWR_UNIT_W          0x0000  /**<  パワークラス単位W                 */
#define	RCD_PWR_UNIT_MW			0x0001	/**<  パワークラス単位mW				*/
#define	RCD_PWR_UNIT_UW			0x0002	/**<  パワークラス単位uW				*/

/* 対応周波数帯 */
#define	RCD_CRP_FRQ_RSV_BIT		0xFFF8	/**<  対応周波数帯予約ビット			*/

/****************************************************************************/
/*	- No.4 -																*/
/*								汎用デファイン								*/
/*																			*/
/****************************************************************************/
#define	RCD_BITMSK0				0x0000	/**<  マスク0							*/
#define	RCD_BITMSK1				0x0001	/**<  マスク1							*/
#define	RCD_BITMSK2				0x0002	/**<  マスク2							*/
#define	RCD_BITMSK3				0x0003	/**<  マスク3							*/
#define	RCD_BITMSK4				0x0004	/**<  マスク4							*/
#define	RCD_BITMSK5				0x0005	/**<  マスク5							*/
#define	RCD_BITMSK6				0x0006	/**<  マスク6							*/
#define	RCD_BITMSK7				0x0007	/**<  マスク7							*/
#define	RCD_BITMSK8				0x0008	/**<  マスク8							*/
#define	RCD_BITMSK9				0x0009	/**<  マスク9							*/
#define	RCD_BITMSK10			0x000A	/**<  マスクA							*/
#define	RCD_BITMSK0X100			0x0100	/**<  マスク100						*/
/* 下位マスク	*/
#define	RCD_LW_MSK				0x00FF	/**<  下位マスク						*/
/* 上位マスク	*/
#define	RCD_HI_MSK				0xFF00	/**<  上位マスク						*/
/* 4bitマスク */
#define	RCD_MSK_000F			0x000F	/**<  0～3bitマスク					*/
/* 4bitマスク */
#define	RCD_MSK_00F0			0x00F0	/**<  4～7bitマスク					*/
/* 4bitマスク */
#define	RCD_MSK_0F00			0x0F00	/**<  8～11bitマスク					*/
/* 4bitマスク */
#define	RCD_MSK_F000			0xF000	/**<  12～15bitマスク					*/
/* 2bitマスク */
#define	RCD_MSK_00C0			0x00C0	/**<  6～7bitマスク					*/

/* 西暦2000年bitマスク	*/
#define	RCD_YEAR2000_MSK		0x2000	/**< 西暦2000年bitマスク				*/
/* ERR情報格納領域最大数(MDIF SV状態管理テーブル) */
#define	RCD_MAX_MDIF_ERR_TTRY	4		/**<  ERR情報格納領域最大数			*/
/* ERR情報格納領域最大数(EXT SV状態管理テーブル) */
#define	RCD_MAX_EXT_ERR_TTRY	2		/**<  ERR情報格納領域最大数			*/
										/* (TTRY:territory)					*/
#define	RCD_IDINF_MAX_NUM		1024	/**<  ID情報最大数						*/
#define	RCD_RSPDAT2				2		/**<  応答データ2byte					*/
#define	RCD_RSPDAT4				4		/**<  応答データ4byte					*/
#define	RCD_RSPDAT6				6		/**<  応答データ6byte					*/
#define	RCD_RSPDAT12			12		/**<  応答データ12byte					*/
#define	RCD_RSPDAT24			24		/**<  応答データ24byte					*/
#define	RCD_RSV_NUM1			1		/**<  予約応答データ数(1)				*/
#define	RCD_RSV_NUM2			2		/**<  予約応答データ数(2)				*/
#define	RCD_RSV_NUM3			3		/**<  予約応答データ数(3)				*/
#define	RCD_RSV_NUM5			5		/**<  予約応答データ数(5)				*/
#define	RCD_RSV_NUM6			6		/**<  予約応答データ数(6)				*/
#define	RCD_RSV_NUM39			39		/**<  予約応答データ数(39)				*/
#define	RCD_RSV_NUM43			43		/**<  予約応答データ数(43)				*/
#define	RCD_RSV_NUM26			26		/**<  予約応答データ数(26)				*/
#define	RCD_MAX_ERRINF_NUM		32		/**<  ERR情報最大数					*/
#define	RCD_MAX_ALMARY_NUM		16		/**<  CPRI応答データのALM最大数		*/


/****************************************************************************/
/* RE制御タスク内使用パラメータ定義											*/
/****************************************************************************/

/* TRA 全制御項目ビットマップ	*/
#define	RCD_TRA_CNTITM_3G			0xEF27	/**<  TRAカードの全制御項目 3G			*/
#define	RCD_TRA_CNTITM_S3G			0x7077	/**<  TRAカードの全制御項目	S3G		*/


/* RE 全カード拡張監視項目ビットマップ	*/
#define	RCD_RE_EXTITM			0x0000	/**<  REの全カード拡張監視項目			*/

/* 無応答ALM応答用 */
#define	RCD_RE_NORES_ALM		0x0280	/**<  無応答ALM発生時のカード情報		*/

#define	RCD_WAIT_TIME_500		50		/**< 500msWait cm_MWait用デファイン	*/

/* 無限ループ用	*/
#define	RCD_LOOP				1		/**<  無限ループ用						*/

/****************************************************************************/
/* RE制御個別ログ定義定数													*/
/****************************************************************************/
#define RCD_LOG_MAX				128			/**<  RE監視個別ログ面数			*/
#define RCD_LOG_SV1_SIZE		64			/**<  RE監視個別ログ1面サイズ		*/

/****************************************************************************/
/* タスク状態定義															*/
/****************************************************************************/
#define RCD_TSK_STN_INI					0		/**<  初期状態					*/
#define RCD_TSK_STN_NORMAL				1		/**<  運用状態					*/


/****************************************************************************/
/* RE dev kind number															*/
/****************************************************************************/
#define RCD_RE_DEV_KIND_NUM					8		/**<  RRE/BRECPRI/BRERF/LRE1T2R/LRE2T2R/LRE1T2RCRD/LRE2T2RCRD					*/

/* TRA SVレジスタ関連 */
#define RCD_USE_BIT_ON			0x0004		/**<  USE bit set ON value in SV A0 */
#define RCD_USE_BIT_OFF			0xE603		/**<  USE bit set OFF value in SV A0 */

/* @} */
/*!
 * @name connect type
 * @{
 */
#define RCD_CONT_3G				0         /**<  the connection is only 3G */
#define RCD_CONT_NOONL3G		1         /**<  not only 3G connection */
#define RCD_CONT_S3G		3         /**<  3G is connection */

#define RCD_LINE_BIT_RATE_MSK         0x0001                                              /**<  the mask of Bit Line Rate */


#define RCD_VARALM_MSK		0x0001         /**<  バリスタALM BIT */
#define RCD_BREAKALM_MSK		0x0002         /**<  BREAK ALM BIT */
#define RCD_FANALM_MSK		0x0004         /**<  FAN ALM BIT */
#define RCD_BRETERMALM_MSK		0x8000         /**<  BRE-TERM ALM BIT */
#define RCD_PLLUNLOCKALM_MSK		0x0002         /**<  PLL UNLOCK ALM BIT */
#define RCD_RFCLKDISCALM_MSK		0x0001         /**<  RF 8k Hz CLK DISC ALM BIT */

/****************************************************************************/
/* 3G BTS share type															*/
/****************************************************************************/
#define	RCD_BTS_SHARE_S3G					0		/**<  S3G only					*/
#define	RCD_BTS_SHARE_CPRI					1		/**<  CPRI share type		*/
#define	RCD_BTS_SHARE_RF						2		/**<  RF share type			*/

/* CeNB-F-083-028 FFCS)Xut modify  start */
//#define	RCD_ERRCODE_SEAR_LEN		14		/**<  Card status report - error code search length	*/
/* CeNB-F-083-028 FFCS)Xut modify  end */
//TD-SRE
#define	RCD_ERRCODE_SEAR_LEN		7		/**<  Card status report - error code search length	*/
//TD-SRE


#define RCD_HDALM_BIT			0x0007		/* ハードALM 検出ビット*/
#define RCD_RSALM_BIT			0x0018		/* リソースALM 検出ビット */
#define RCD_SOFTALM_BIT			0x2000		/* Soft ALM 検出ビット */
#define RCD_SOFTALM2_BIT        0x4000      /* Soft ALM 2 検出ビット */
#define RCD_CARDALM_BIT         0x2700

#define RCD_NOTINS_BIT_ON		CMD_NOTINS_BIT_ON
#define RCD_ACT_BIT				CMD_ACT_BIT			/* カードACTビット				*/
#define RCD_ALM_BIT_ON			CMD_ALM_BIT_ON		/* ALM bit set ON value in SV A0/A1 */
#define RCD_ERR_BIT_ON			CMD_ERR_BIT_ON		/* ERR bit set ON value in SV A0/A1 */
#define RCD_BLK_BIT_ON			CMD_BLK_BIT_ON		/* BLK bit set ON value in SV A0/A1 */
#define RCD_CHK_BIT_ON			CMD_CHK_BIT_ON		/* CHK bit set ON value in SV A0/A1 */
/* #define RCD_USE_BIT_ON		CMD_USE_BIT_ON	*/	/* USE bit set ON value in SV A0/A1 */
#define RCD_PA_ERR_BIT			CMD_PA_ERR_BIT		/* PA-OFF ERR bit in SV A6/A7*/

#define RCD_CLR_ERRBIT1			CMD_CLR_ERRBIT1		/**< Bit 1 clear	(ERR)	*/

#define RCD_3GL3ERR_ON		0x0002	/* 3G ERR bit set ON in SV A8/A9 */
#define RCD_S3GL3ERR_ON		0x0004	/* S3G ERR bit set ON in SV A8/A9 */

#define RCD_3GERR_OFF		0x1FFD	/* 3G ERR bit set OFF in SV A8/A9 for L3 error AND file error */
#define RCD_S3GERR_OFF		0xE7FB	/* S3G ERR bit set OFF in SV A8/A9 for L3 error AND file error */
/* CeNB-F-083-028 FFCS)Xut modify  start */
#define RCD_3GERR		0xE778
//3.5-SRE
#define RCD_S3GERR      0x19F5
/* CeNB-F-083-028 FFCS)Xut modify  end */
#define RCD_CARDSV_ERR_BIT		CMD_CARDSV_ERR_BIT
#define RCD_CARDSV_ALM_BIT		CMD_CARDSV_ALM_BIT
#define RCD_CARDSV_ACT_BIT		CMD_CARDSV_ACT_BIT
#define RCD_CARDSV_BLK_BIT		CMD_CARDSV_BLK_BIT
#define RCD_CARDSV_BLK_BIT_3G	CMD_CARDSV_BLK_BIT_3G

/* Card FB define */
#define RCD_TDD_FB_NUM				0x0007
#define RCD_CARDFB_BIT				0x0800  /**< D11: TRX-INF */
#define	RCD_FB_DUP         			0 		/**< 0：DUP             */
//TDD-SRE
#define	RCD_FB_BPF_SWITCH         	0 		/**< 0:BPF/SWITCH            */

//TDD-SRE
#define	RCD_FB_LNA         			1 		/**< 1：LNA             */
#define	RCD_FB_T_PA        			2 		/**< 2：T_PA            */
#define	RCD_FB_TRX         			3 		/**< 3：TRX             */
#define	RCD_FB_TRX_INF     		4 		/**< 4：TRX_INF         */
#define	RCD_FB_OA_RA_INF   		17		/**< 17：OA_RA_INF      */
#define	RCD_FB_TILT_INF    		18		/**< 18：TILT_INF       */
#define	RCD_FB_PORT_INF    		19		/**< 19：PORT_INF       */
#define	RCD_FB_MT_INF      			20		/**< 20：MT_INF         */
#define	RCD_FB_3GRF_INF    		25		/**< 25：3GRF_INF       */
#define	RCD_FB_EX_OA_RA_INF	26		/**< 26：EX_OA_RA_INF   */
#define	RCD_FB_CLK_INF     		27		/**< 27：CLK_INF        */
#define	RCD_FB_NUM     			29		/**< 機能部数29(0～28)     */
#define	RCD_FB_FHM_NUM     		12		/**< 機能部数12     */

/* Card FB ALM TYPE define */
#define	RCD_FB_ALM_NOTINS         	0 			/**< 0：未実装	*/
#define	RCD_FB_ALM_NORMAL         	1 			/**< 1：Normal    */
#define	RCD_FB_ALM_HIGH        		2 			/**< 2：High-ALM  */
#define	RCD_FB_ALM_SYS         		3 			/**< 3：SYS-ALM   */

#define	RCD_FB_ALM_FLG_NOTINS		0x0001 			/**< 未実裁E*/
#define	RCD_FB_ALM_FLG_NORMAL		0x0002 			/**< Normal    */
#define	RCD_FB_ALM_FLG_HIGH			0x0004 			/**< High-ALM  */
#define	RCD_FB_ALM_FLG_SYS			0x0008 			/**< SYS-ALM   */

/* Card FB resource ALM number define */
//#define	RCD_FB_RESALM_NUM         		20 			/**< resource ALM number   */
/* Card FB carrier ALM number define */

/* Card FB resource ALM number define */
#define	RCD_FB_RESALM_NUM         		52			/**< resource ALM number   */
/* Card FB carrier ALM number define */


#define	RCD_FB_CARALM_NUM         		49			/**< carrier ALM number   */
#define	RCD_FB_RESALM_SET         		0x0200
#define	RCD_FB_RESALM_MASK         		0xFDFF
#define	RCD_FB_ACT_MASK         		0xFFDF
#define	RCD_FB_USE_MASK         		0xFBFF

/* UL/DL Carrier define */
#define RCD_CAR_UL		0		/* UL carrier */
#define RCD_CAR_DL		1		/* DL carrier */

/* 送信系 define */
#define RCD_SND_BR_0	1		/* 0 系送信 */
#define RCD_SND_BR_1	2		/* 1 系送信 */

/* TD-SRE TRX setting S. */
/* 送信系 define */
/* All bit open */
#define RCD_SND_BR_ALL	0x0F		/* 0/1/2/3 系送信 */
/* TD-SRE TRX setting E. */

/* スリープ制御 define */
#define RCD_SLP            2		/* スリープ制御 */

/* スリープ状態 - S3G カード拡張制御項目	 */
#define RCD_S3G_FI_SLEEP_OFF			0x0010		/* スリープ解除  */
#define RCD_S3G_FI_SLEEP1_ON			0x0008		/* スリープ（1系） */
#define RCD_S3G_FI_SLEEP0_ON			0x0004		/* スリープ（0系）  */
#define RCD_S3G_FI_SLEEP_ON				0x0002		/* スリープ */

//TD-SRE
//TDD-SRE
#define RCD_S3G_FI_SLEEP3_ON			0x0040
#define RCD_S3G_FI_SLEEP2_ON			0x0020
//TDD-SRE


/* スリープ状態 - 3G 基本監視項目	 */
#define RCD_3G_SV_SLEEP_BIT_ON			0x3000		/* sleep bit set ON */
#define RCD_3G_SV_SLEEP1_BIT_ON			0x2000		/* sleep1 bit set ON */
#define RCD_3G_SV_SLEEP0_BIT_ON			0x1000		/* sleep0 bit set ON */

/* スリープ状態 - 3G 機能部固有制御項目	 */
#define RCD_3G_FI_SLEEP_OFF				0x0080		/* スリープ解除  */
#define RCD_3G_FI_SLEEP1_ON				0x0040		/* スリープ（1系） */
#define RCD_3G_FI_SLEEP0_ON				0x0020		/* スリープ（0系）  */
#define RCD_3G_FI_SLEEP_ON				0x0010		/* スリープ */
#define RCD_3G_FI_ALL_SLEEP				0x00f0		/* ALL スリープ */

/* スリープ状態 - S3G カード拡張監視項目	 */
#define RCD_S3G_EXT_SLEEP0				0x0010		/* スリープ（0系） */
#define RCD_S3G_EXT_SLEEP1				0x0020		/* スリープ（1系）  */
#define RCD_S3G_EXT_SLEEP				0x0030		/* スリープ */

/* スリープ状態 - S3G TPA機能部拡張監視項目	 */
#define RCD_S3G_FB_EXT_SLEEP			0x0044		/* スリープ + AMP部電源断 */
/* スリープ状態 - S3G TPA機能部ブランチ拡張監視項目MASK	 */
#define RCD_S3G_FB_EXT_SLEEP_MSK		0xffbf		/* スリープ mask*/
/* AMP部電源断 - S3G TPA機能部ブランチ拡張監視項目MASK	 */
#define RCD_S3G_FB_EXT_AMPDOWN_MSK		0xfffb		/* AMP部電源断 mask*/

#define RCD_PAOFF_BR0			0x0100		/* PAOFF(BR0) bit in SV A6/A7 */
#define RCD_PAOFF_BR1			0x0200		/* PAOFF(BR1) bit in SV A6/A7 */
#define RCD_PAOFF_BR0_1			0x0300		/* PAOFF(BR0/1) bit in SV A6/A7 */

#define RCD_CARDCNT_RMTFRCBLK	0x0004
#define RCD_CARDCNT_RMTUNBLK	0x0010
#define RCD_CARDCNT_RMTBLK		0x0020
#define RCD_CARDCNT_LCLUNBLK	0x1000
#define RCD_CARDCNT_LCLBLK		0x2000
#define RCD_BLKCNT_RMT		(RCD_CARDCNT_RMTFRCBLK | RCD_CARDCNT_RMTUNBLK |RCD_CARDCNT_RMTBLK)
#define RCD_BLKCNT_LCL		(RCD_CARDCNT_LCLUNBLK |RCD_CARDCNT_LCLBLK)
/** @name table max index define */
/* @{ */

/* ======================================================================== */
/* テーブル要素数定義定数													*/
/* ======================================================================== */
#define RCD_SVTBL_NUM			5			/* SVテーブルの要素数			*/
//TD-SRE
#define RCD_PLLALM_NUM		1			/* minor alarm factor number of PLL ALARM */
#define RCD_CPRIALM_NUM		3			/* minor alarm factor number of CPRI ALARM */
#define RCD_APDCOMALM_NUM	11			/* minor alarm factor number of APD COM ALARM */
#define RCD_APDALM_NUM		10			/* minor alarm factor number of APD0/APD1/APD2/APD3 ALARM */
#define RCD_APDALM_LOOP_NUM 16			/*  (apd com apd0 apd1 apd2 apd3) ALM code convert loop number*/
//TD-SRE
#define RCD_3GRFINFALM_NUM	0			/* alarm factor number of 3GRFINF ALARM */
#define RCD_EXTIFTBL_NUM		0			/* EXT interface table number*/
/* @} */

/** @name card state define */
/* @{ */

/* ======================================================================== */
/* カード状態																*/
/* ======================================================================== */
#define	RCD_NOINS				0			/* 未実装						*/
#define	RCD_INS					1			/* 実装							*/
#define	RCD_ACT					3			/* ACT							*/
#define	RCD_USE					4			/* USE							*/
#define	RCD_ALM					5			/* ALM							*/
#define	RCD_ERR					6			/* ERR							*/
#define	RCD_ERROFF				7			/* ERR解除						*/
#define	RCD_BLK					8			/* 閉塞							*/
#define	RCD_BLKOFF				9			/* 閉塞解除						*/
#define	RCD_TST					10			/* 診断							*/
#define	RCD_TSTOFF				11			/* 診断終了						*/
#define	RCD_LCL					15			/* ローカル						*/
#define	RCD_RMT					16			/* リモート						*/
#define	RCD_ACTIVE				17			/* Active						*/
#define	RCD_NEGATIVE			18			/* Negative						*/
#define	RCD_HI_Z				19			/* Hi-Z							*/
#define	RCD_NOFACT				20			/* 要因なし						*/
#define	RCD_UNUSE				21			/* 未使用						*/
#define RCD_AMPPWDSC			22			/* AMP 部電源断 				*/
#define	RCD_ELSE				30			/* その他エラー					*/
/* @} */

/****************************************************************************/
/* 走行履歴情報テーブル用													*/
/****************************************************************************/
#define RCD_RUNHIS_FLG_RCV		0x00	/* 送受信フラグ(受信)   */
#define RCD_RUNHIS_FLG_SND		0x01	/* 送受信フラグ(送信)   */
#define RCD_RUNHIS_FLG_SND_NG	0xff	/* 送受信フラグ(送信NG) */
/* @} */

/* TD-SRE TRX setting S. */
/****************************************************************************/
/* carrier set/release command													*/
/****************************************************************************/
enum
{
	RCD_CARR_SET_CMD = 0,										/* carrier set */
	RCD_CARR_REL_CMD											/* carrier release*/
};
/****************************************************************************/
/* carrier#n bandwith bit shift													*/
/****************************************************************************/
#define RCD_CARR_BW_1_REG_SHIFT	0x00							/* bandwith carrier#1 bit shift  */
#define RCD_CARR_BW_2_REG_SHIFT	(RCD_CARR_BW_1_REG_SHIFT+0x03)	/* bandwith carrier#2 bit shift  */
#define RCD_CARR_BW_3_REG_SHIFT	(RCD_CARR_BW_2_REG_SHIFT+0x03)	/* bandwith carrier#3 bit shift  */
#define RCD_CARR_BW_4_REG_SHIFT	(RCD_CARR_BW_3_REG_SHIFT+0x03)	/* bandwith carrier#4 bit shift  */
#define RCD_CARR_BW_5_REG_SHIFT	(RCD_CARR_BW_4_REG_SHIFT+0x03)	/* bandwith carrier#5 bit shift  */

/****************************************************************************/
/* carrier#n bandwith bit on														*/
/****************************************************************************/
#define RCD_CARR_BW_1_REG_BIT_ON	0x07						/* bandwith carrier#1 bit on  */


/****************************************************************************/
/* carrier#1~n bandwith summation														*/
/****************************************************************************/
#define RCD_CARR_BW_ALL_SUM_NT2R	0x10						/* bandwith all summation(NT2R) */
#define RCD_CARR_BW_ALL_SUM_NT4R	0x20						/* bandwith all summation(NT4R) */

#define RCD_C0BR_TRXSET			0x2222							/*if c0 branch trx set*/
#define RCD_C1BR_TRXSET			0x4444							/*if c0 branch trx set*/


#define RCD_DEV_TYPE_NT2R_RATE		0x02						/* device type rate(NT2R) */
#define RCD_DEV_TYPE_NT4R_RATE		0x04						/* device type rate(NT4R) */

/****************************************************************************/
/*	TDD uplink/downlink configuration max									*/
/*	TDD special subframe configuration max  								*/
/*	TDD downlink cyclic prefix length max									*/
/****************************************************************************/
#define RCD_UPDWN_LINK_CFG_MAX		0x06
#define RCD_SPE_SF_CFG_MAX			0x09
#define RCD_DWNLK_CP_LEN_MAX		0x01

/* NT2R:0 NT4R:1 */
enum
{
	BR_BITMAP_NO_CHK = 0,				/* no check				*/
	BR_BITMAP_CHK						/* check				*/
};

/* have err or not */
enum
{
	RCD_HAVE_ERR = 0,				/* have err			*/
	RCD_NOT_ERR						/* not  err			*/
};

/*find alm or not find*/
enum
{
	RCD_FIND_RES_ALM = 0,			/* have resource alm	*/
	RCD_NOT_FIND_RES_ALM			/* have not resorce alm	*/
};

/* TRX rev branch No. */
#define BRANCH0 0
#define BRANCH1 1
#define BRANCH2 2
#define BRANCH3 3
#define RX_ONLY  0xFF

/*FHM from here*/

/**
*
* @name ON/OFF状諷・*/
/** ON状諷・*/
#define D_L3_OFF	0
/** OFF状諷・*/
#define D_L3_ON		1

/**
*
* @name MT接続状諷・*/
/** 未接邯・*/
#define D_L3_MT_NOT_CONNECT	0
/* 接邯・*/
#define D_L3_MT_CONNECT		1

/**
*
* @name Layer3状態が
*/
/**  */
#define D_L3_STA_4	4
/**  */
#define D_L3_STA_2	2
//REC未接続
#define D_L3_STA_REC_IDLE	6
//REC接続済
#define D_L3_STA_REC_USING	7

/* @} */
/* TD-SRE TRX setting E. */

/* @} */

/* @} */
/* TD-SRE TRX setting E. */

/* @} */

/**
*
* @name 送信状諷
*/
/** 送信状諷・未送信) */
#define D_L3_NOT_EXE	0
/** 送信状諷・処逅・ｸｭ) */
#define D_L3_EXE		1


/**
*
*@name  応答征E■
*/
/** 応答未征E■ */
#define D_L3_NOT_RES_WAIT	0
/** 応答征E■ */
#define D_L3_RES_WAIT		1


/**
*
*@name FLD-MT接続状諷
*/
/** 未接邯・*/
#define D_L3_FLDMT_NOT_CONN		0
/** 接邯・*/
#define D_L3_FLDMT_CONN			1

/** @name  外部装置データ送信中状態 */
/* @{ */
/* 未処理 */
#define RCD_EXTSNDREQ_IDLE			0
/* AISG送信中 */
#define RCD_EXTSNDREQ_AISG			1
/* 非AISG送信中 */
#define RCD_EXTTSNDREQ_NONEAISG		2
/* @} */

/** @name 信号送信対象RE */
/* @{ */
/* 未処理 */
#define RCD_SIGSNDRE_IDLE		0
/* 送信処理中 */
#define RCD_SIGSNDRE_RUNNING	1
/* OK応答受信 */
#define RCD_SIGSNDRE_OK			2
/* NG応答受信 */
#define RCD_SIGSNDRE_NG			3
/* @} */

/**
*
* @name 受信バイト数
*/
/** 受信バイト数 3G */
#define MIN_MSG3G_SIZE		1
#define MAX_MSG3G_SIZE		2048

/** 受信バイト数 LTE */
#define MIN_MSGLTE_SIZE		1
#define MAX_MSGLTE_SIZE		10240

/**
*
* @name FLD-MTデータファイル転送受信要求の受信バイト数
*/
/** バイト数最小 */
#define FLDMT_FIL_TRANS_DAT_MIN		1
/** バイト数最大 */
#define FLDMT_FIL_TRANS_DAT_MAX		4096

/**
*
* @name TILT-INF信号種別
*/
/* @{ */
/* 状態要求信号 */
#define STATE_REQ_SIG			0x0001
/* 局リセット要求信号 */
#define	RESET_REQ_SIG			0x00C9
/* 配下REリセット要求信号 */
#define SUBRE_RESET_REQ_SIG		0x0005
/* 局ON/OFF要求信号 */
#define	ONOFF_REQ_SIG			0x0088
/* 配下REON/OFF要求信号 */
#define SUBRE_ONOF_SIG			0x0004
/* ヘルスチェック要求信号 */
#define HEAL_CHK_REQ_SIG		0x0000
/* 状態報告信号 */
#define STATE_RES_SIG			0x0011
/* 局リセット報告信号 */
#define	RESET_RES_SIG			0x0019
/* 配下REリセット報告信号 */
#define SUBRE_RESET_RES_SIG		0x0015
/* 局ON/OFF報告信号 */
#define	ONOFF_RES_SIG			0x0018
/* 配下REON/OFF報告信号 */
#define SUBRE_ONOFF_RES_SIG		0x0014
/* ヘルスチェック報告信号 */
#define HEAL_CHK_RES_SIG		0x0010
/* 状態変化信号 */
#define CHANGE_STATE_RES_SIG	0x0012
/* 状態変化信号 (for 1st health check) */
#define FULL_STATE_RES_SIG		0x0013
/* @} */


/* 装置番号(E) */
#define DEV_NUM_MIN				1
#define DEV_NUM_MAX				16

/* 信号長 */
#define MAX_SIGNAL_LENGHT		244

/* 外部装置番号がTILT-INF	*/
#define EXTDEV_NUM_TILTINF_LTE	2
#define EXTDEV_NUM_TILTINF_3G	4

/* 送信バイト数 */
#define MIN_EXTDAT_MSGSND_SIZE	1
#define MAX_EXTDAT_MSGSND_SIZE	2048

/*	装置種別(FHM宛)	*/
#define DEV_TYPE_FHM	0x97

/** @name 外部装置データ受信中状態 */
/* @{ */
/* 未処理 */
#define RCD_EXTRCVREQ_IDLE		0
/* RE主導外部装置データ受信要求処理中 */
#define RCD_EXTRCVREQ_RESND		1
/* FHM主導の状態報告信号処理中 */
#define RCD_EXTRCVREQ_STAREP	11
/* FHM主導の局リセット信号処理中 */
#define RCD_EXTRCVREQ_SYSRST	12
/* FHM主導の配下REリセット報告信号処理中 */
#define RCD_EXTRCVREQ_RERST		13
/* FHM主導の局ON/OFF報告信号処理中 */
#define RCD_EXTRCVREQ_SYSONOFF	14
/* FHM主導の配下REON/OFF報告信号処理中 */
#define RCD_EXTRCVREQ_REONOFF	15
/* FHM主導の配下ヘルスチェック応答信号処理中 */
#define RCD_EXTRCVREQ_HCRSP		16
/* FHM主導の状態変化信号処理中 */
#define RCD_EXTRCVREQ_STACHG	17
/* @} */

/** @name  REカード機能部情報 */
/* @{ */
/* REカード機能部情報 ( 3G )*/
#define CMD_CHK_TRXINF_3G		0x0000
 /* REカード機能部情報 ( LTE )*/
#define CMD_CHK_TRXINF_LTE		0x0800
/* @} */

#define ALM_D15_BIT 0x8000
#define ALM_D14_BIT 0x4000
#define ALM_D13_BIT 0x2000
#define ALM_D12_BIT 0x1000
#define ALM_D7_BIT 0x80
#define ALM_D6_BIT 0x40
#define ALM_D5_BIT 0x20
#define ALM_D4_BIT 0x10
#define ALM_D3_BIT 0x08
#define ALM_D2_BIT 0x04
#define ALM_D1_BIT 0x02
#define ALM_D0_BIT 0x01

#define RCD_CARDSV_IMP_BIT		0x0080

#define CPRI_STATE_A 0
#define CPRI_STATE_B 1
#define CPRI_STATE_C 2
#define CPRI_STATE_D 3
#define CPRI_STATE_E 4
#define CPRI_STATE_F 5

#define	RCD_STX					0x02
#define	RCD_ETX					0x03

#define RCD_TILTHC_RCV 1
#define RCD_TILTHC_NONE 0

#define D_RETRY_COUNTER_INIT 0
#define D_RETRY_COUNTER_MAX 3

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  BCDコード変換
 */
#define D_L3_CNVBCD(hex) (\
    ((((hex)/10000000)   )<<28)+ \
    ((((hex)/ 1000000)%10)<<24)+ \
    ((((hex)/  100000)%10)<<20)+ \
    ((((hex)/   10000)%10)<<16)+ \
    ((((hex)/    1000)%10)<<12)+ \
    ((((hex)/     100)%10)<< 8)+ \
    ((((hex)/      10)%10)<< 4)+ \
    ((((hex)         )%10)    )  \
)

/*
 *@brief Convert BCD to DEC
 */
#define D_L3_CNVBCD_DEC(num) ((( num & 0xF0) >> 4 ) * 10 +(num & 0x0F) )

/*
 *@brief BCD format Check. 0: NG, 1: OK
 */
#define D_L3_CHKBCD(num) ( ((((num&0xF0)>>4)>9)||((num&0x0F)>9)) ? D_RRH_NG: D_RRH_OK )

/* eeprom sfp  data */
#define D_CPR_EEPROM_SFP_MAX		10
#define D_CPR_EEPROM_SFP_LEN		12


/*16B Slot Over Check Count */
#define RCD_OVER_MAX_SLOT			64

#endif /* M_RC_DEF_HEAD */
