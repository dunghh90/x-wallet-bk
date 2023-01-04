/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_def.h
 *  @brief	TRA process Common Definition
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2008/11/19 FFCS)Tangj modified for M-S3G-eNBPF-00877
 *  @date   2008/11/27 FFCS)Tangj modified for M-S3G-eNBSYS-00011
 *  @date   2009/04/16 FFCS)Wuh modify for S3G PRC CR-00038-005 (CPRI spec V1.07)
 *  @date   2009/07/16 FFCS)Wuh modify for CMD_SEMID_SERIALBUS for for M-S3G-eNBPF-01995 
 *  @date   2009/09/08 FFCS)Tangj modify CMD_S3G_FREQ_MAX and CMD_S3G_FREQ_MIN for M-S3G-eNBPF-02442
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/10/14 QNET)Kabasima HISTORY ADD CR-00064-001(TRA-HS-V2R9 HDLC RATE CHANGE IRQ)
 *  @date   2009/10/20 QNET)Kabasima ADD CR-00073-000[DCM向け]RE起動時の光MDL初期化設定の追加
 *  @date   2010/01/05 FJT)Taniguchi MOD M-S3G-eNBPF-02875
 *  @date   2010/01/13 QNET)Kabasima MOD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充追加修正
 *  @date   2010/01/20 QNET)Kabasima MOD M-S3G-eNBPF-03127:[品質改善]RE障害ログ収集改善
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *  @date   2010/04/02 FJT)Taniguchi CR-XXXXX-XXX:F-971(DCM)対応
 *  @date   2010/04/27 FJT)Otani     CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2010/08/03 CMS)Yorozuya  CR-00014-000 : ハソV2.19-02版
 *  @date   2010/09/17 CMS)Takahashi M-S3G-eNBPF-03763
 *  @date   2010/10/04 CMS)Kudou     RE Warm UP時間変更
 *  @date   2011/02/25 FJT)Tokunaga  modify for 1.5G-RRE対応
 *  @date   2011/05/06 FJT)Koshida M-S3G-eNBPF-04042:REファイルの運用中ダウンロードと障害ログ取得の競合にてTRAにALM発生
 *  @date   2011/07/21 FJT)Tokunaga M-S3G-eNBPF-04059:[IT3]REリモート閉塞時の(3003)カード状態監視開始マクロメール通知について
 *  @date   2011/08/18 FJT)Tokunaga CR-00043-000 FAN ALM問題対応
 *  @date   2011/09/16 CMS)Uchida   11B-02-005：VA提案の2G/800MのSLCアーキ適用対応
 *  @date   2011/09/23 FFCS)Linlj  modify for 省電力対応
 *  @date   2012/03/15 ALP)Yamamoto 12A-01-006/M-S3G-eNBSYS-02601：SLC-RREにおけるALM/ERRコード処理変更について
 *  @date   2012/07/27 FFCS)niemsh  modify for 1.5GVA
 *  @date   2012/08/15 ALP)Fujiiy  modify for M-S3G-eNBPF-04174
 *  @date   2012/08/21 ALP)Yokoyama modify for M-S3G-eNBPF-04192
 *  @date   2012/08/29 ALP)Yokoyama modify for M-S3G-eNBPF-04207
 *  @date   2012/09/07 FFCS)Xut modify for 1chip for 1.7G/700M
 *  @date   2012/09/12 CMS)Nakajima modify for M-S3G-eNBPF-04228
 *  @date   2012/10/05 CMS)Nakajima modify for M-S3G-eNBPF-04265
 *  @date   2012/11/05 CMS)Nakajima modify for M-S3G-eNBPF-04302
 *  @date   2013/02/18 CMS)Andou XXXXXXXX TRX設定応答のコード変更
 *  @date   2013/03/03 FFCS)niemsh Toffset変更
 *  @date   2015/04/25 FFCS)zhangxq TDD
 *  @date   2015/07/27 ALP)Ueda modify for TDD-SRE(Zynq
 *  @date   2015/09/10 ALP)Ueda modify for TDD-RRE(Zynq)
 *	@date   2015/09/14 ALP)kamada  modify for RRE 3.5G TDD
 *  @date   2015/10/07 FJT)satoK Modify for 3.5G-TDD(RRE)
 *  @date   2015/11/05 ALP)ueda modify for TDD-RRE(Zynq)
 *  @date   2015/11/07 ALP)murakami modify for TDD-RRE(Zynq)
 *  @date   2016/02/19 ALP)fujiiy modify for 16A
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/*****************************************************************************/

/* For multiple include */
#ifndef M_CM_DEF 
#define M_CM_DEF 

/* TimerID等の置き換えのために以下を取り込む	*/
#include "f_rrh_def.h"

/** @addtogroup RRH_DCM
 *  @{
 */
/****************************************************************************/
/*	デファイン定義															*/
/*	m_cm_def.hは以下の構成をとっている										*/
/*		No.1：CPRI信号用パラメータデファイン定義							*/
/*		No.2：CPRI信号種別/データ部レングス定義								*/
/*		No.3：タスクID														*/
/*		No.4：タイマID														*/
/*		No.5：起動要因コード												*/
/*		No.6：カード内共通関数のパラメータ									*/
/*		No.7：カード内共通レジスタアドレス/マスク							*/
/*			No.7_1：自カードレジスタ関連									*/
/*			No.7_2：APD-SPI related register								*/
/*			No.7_4：その他のレジスタ関連									*/
/*		No.8：カード内共通使用デファイン値									*/
/*			No.8_1：タスクIFのデファイン定義								*/
/*			No.8_2：その他													*/
/*		No.9：TRAカード内共通テーブル関連定義								*/
/*																			*/
/* (それぞれにラベルがあるので、追記する際はその範囲内への記載を規則とする)	*/
/****************************************************************************/

/****************************************************************************/
/*	- No.1 -																*/
/*																			*/
/*				CPRI信号用パラメータデファイン定義							*/
/*																			*/
/****************************************************************************/
#define CMD_LINK_NUM_T	1
/*****************************/
/* RE PORT制御要求パラメータ */
/*****************************/
/* 情報ビット */
#define CMD_MIN_INFOBIT			1			/**< 最小情報ビット				*/
#define CMD_MAX_INFOBIT			8			/**< 最大情報ビット				*/
#define CMD_MAX_INFOBIT_FACT	4			/**< 最大情報ビット	for real machine*/
#define CMD_INVADDR_MAX		0x003B			/* インベントリアドレス		*/

/* 制御 */
#define CMD_RLYOFF				1			/**< リレーOFF					*/
#define CMD_RLYON				2			/**< リレーON					*/

/******************************/
/* REバージョン報告パラメータ */
/******************************/
/* メジャーバージョン/マイナーバージョン */
#define CMD_VERNO_MAX			99			/**< 最大バージョン番号			*/

/* 情報数 */
#define CMD_VERINFNUM_MAX		16			/**< バージョン情報最大数		*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* 年月日 */
#define CMD_VERYERSET_INT		0x2000		/**< バージョン情報年初期値	*/

#define CMD_VERMON_DAYSET_INT		0x0101		/**< バージョン情報月日初期値	*/

/******************************/
/* REバージョン応答パラメータ */
/******************************/
#define CMD_VER_OFF					1
#define CMD_VER_UNLINK				2
#define CMD_VER_NG					3
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**********************************/
/* ファイル情報報告通知パラメータ */
/**********************************/
/********************************************/
/* ファイルデータ送信完了報告通知パラメータ */
/********************************************/
/* 対象指定 */
#define CMD_FRAME			1			/**< 架						*/
#define CMD_CARD			3			/**< カード					*/
#define CMD_FAN				4			/**< FAN					*/

/* ID情報識別 */
#define CMD_HARD			1			/**< ハードウェア			*/

/* 対象データ */
#define CMD_SYSPAR			1			/**< システムパラメータ		*/
#define CMD_FRM				2			/**< ファームウェア			*/
#define CMD_FRM_NONEW		3			/**< ファームウェア更新なし	*/
#define CMD_LOG				4			/**< 障害ログ				*/

/*********************************/
/*	REスロット情報報告パラメータ */
/*********************************/
/* REカード機能部情報 */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CRDFNBIT_DUP		0x0080		/**< DUP/BPF/SWITCH	(D07)		*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CRDFNBIT_SV			0x0040		/**< SV-INF		(D06)			*/
#define CMD_CRDFNBIT_FLD		0x0020		/**< FLD-INF		(D05)		*/
#define CMD_CRDFNBIT_PROT		0x0010		/**< PORT-INF		(D04)		*/
#define CMD_CRDFNBIT_TILT		0x0008		/**< TILT-INF		(D03)		*/
#define CMD_CRDFNBIT_MTT		0x0004		/**< MTT-INF		(D02)		*/
#define CMD_CRDFNBIT_OARA		0x0002		/**< OA-RA-INF	(D01)			*/
#define CMD_CRDFNBIT_SCT		0x0001		/**< REセクタ部	(D00)			*/

#define CMD_CRDFNBIT_VBIT	(\
			CMD_CRDFNBIT_SV		|\
			CMD_CRDFNBIT_FLD	|\
			CMD_CRDFNBIT_PROT	|\
			CMD_CRDFNBIT_TILT	|\
			CMD_CRDFNBIT_MTT	|\
			CMD_CRDFNBIT_OARA	|\
			CMD_CRDFNBIT_SCT	)			/**< 有効ビット					*/

/* パラメータチェック */
#define CMD_SLOTINF_MAX	16					/**< スロット情報最大数			*/
#define CMD_MINSLOTNO	1					/**< 最小SLOT番号				*/
#define CMD_MAXSLOTNO	56					/**< 最大SLOT番号				*/


/******************************/
/* REカード状態報告パラメータ */
/******************************/
/* カード基本監視項目 */
#define CMD_CRDSVBIT_PSTBY		0x0800		/**< PSTBY(D11)					*/
#define CMD_CRDSVBIT_USE		0x0400		/**< USE(D10)					*/
#define CMD_CRDSVBIT_ALM		0x0200		/**< ALM(D9)					*/
#define CMD_CRDSVBIT_ERR		0x0100		/**< ERR(D8)					*/
#define CMD_CRDSVBIT_EQP		0x0080		/**< 実装(D7)					*/
#define CMD_CRDSVBIT_UNEQP		0x0040		/**< 未実装(D6)					*/
#define CMD_CRDSVBIT_ACT		0x0020		/**< ACT(D5)					*/
#define CMD_CRDSVBIT_STBY		0x0010		/**< STBY(D4)					*/
#define CMD_CRDSVBIT_PBLK		0x0008		/**< PBLK(D3)					*/
#define CMD_CRDSVBIT_BLK		0x0004		/**< BLK(D2)					*/
#define CMD_CRDSVBIT_CHECK		0x0002		/**< CHECK(D1)					*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CRDSVBIT_NOEQP		0x0000		/**< 未実装						*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_CRDSVBIT_VBIT	(\
			CMD_CRDSVBIT_PSTBY	|\
			CMD_CRDSVBIT_USE	|\
			CMD_CRDSVBIT_ALM	|\
			CMD_CRDSVBIT_ERR	|\
			CMD_CRDSVBIT_EQP	|\
			CMD_CRDSVBIT_UNEQP	|\
			CMD_CRDSVBIT_ACT	|\
			CMD_CRDSVBIT_STBY	|\
			CMD_CRDSVBIT_PBLK	|\
			CMD_CRDSVBIT_BLK	|\
			CMD_CRDSVBIT_CHECK	)			/**< 有効ビット					*/

/* カード拡張監視項目 */
#define CMD_CRDEVBIT_AMPPOFF	0x8000		/**< AMP電源断	(D15)			*/
#define CMD_CRDEVBIT_WARMUP		0x0002		/**< WarmUp		(D01)			*/

#define CMD_CRDEVBIT_VBIT	(\
			CMD_CRDEVBIT_AMPPOFF	|\
			CMD_CRDEVBIT_WARMUP		)		/**< 有効ビット					*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* カード基本制御項目（LTE） */
#define CMD_CRDCTBIT_L_RST_LTE		0x4000		/**< ローカルリセット	(D14)		*/
#define CMD_CRDCTBIT_L_BLK_LTE		0x2000		/**< ローカル閉塞		(D13)		*/
#define CMD_CRDCTBIT_AUTO_RST_LTE	0x0100		/**< 自律リセット		(D08)		*/
#define CMD_CRDCTBIT_R_RST_LTE		0x0040		/**< リモートリセット	(D06)		*/
#define CMD_CRDCTBIT_R_BLK_LTE		0x0020		/**< リモート閉塞		(D05)		*/
#define CMD_CRDCTBIT_L_BLKOFF_LTE	0x0010		/**< ローカル閉塞解除	(D04)		*/
#define CMD_CRDCTBIT_R_NECHG_LTE	0x0008		/**< リモートNE切替		(D03)		*/
#define CMD_CRDCTBIT_R_CMPBLK_LTE	0x0004		/**< リモート強制閉塞	(D02)		*/
#define CMD_CRDCTBIT_R_ALM_LTE		0x0002		/**< リモートALM		(D01)		*/
#define CMD_CRDCTBIT_R_CHK_LTE		0x0001		/**< リモート診断		(D00)		*/

#define CMD_CRDCTBIT_LTE_VBIT	(\
			CMD_CRDCTBIT_L_RST_LTE		|\
			CMD_CRDCTBIT_L_BLK_LTE		|\
			CMD_CRDCTBIT_AUTO_RST_LTE	|\
			CMD_CRDCTBIT_R_RST_LTE		|\
			CMD_CRDCTBIT_R_BLK_LTE		|\
			CMD_CRDCTBIT_L_BLKOFF_LTE	|\
			CMD_CRDCTBIT_R_NECHG_LTE	|\
			CMD_CRDCTBIT_R_CMPBLK_LTE	|\
			CMD_CRDCTBIT_R_NECHG		|\
			CMD_CRDCTBIT_R_BLKOFF		|\
			CMD_CRDCTBIT_R_ALM_LTE		|\
			CMD_CRDCTBIT_R_CHK_LTE		)		/**< 有効ビット					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_CRDCTBIT_L_BLKOFF	0x8000		/**< ローカル閉塞解除	(D15)		*/
#define CMD_CRDCTBIT_L_BLK		0x4000		/**< ローカル閉塞		(D14)		*/
#define CMD_CRDCTBIT_L_RST		0x2000		/**< ローカルリセット	(D13)		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CRDCTBIT_AUTO_RST	0x1000		/**< 自律リセット		(D12)		*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CRDCTBIT_R_ALM		0x0800		/**< リモートALM		(D11)		*/
#define CMD_CRDCTBIT_R_ERR		0x0400		/**< リモートERR		(D10)		*/
#define CMD_CRDCTBIT_R_ERROFF	0x0200		/**< リモートERR回復	(D09)		*/
#define CMD_CRDCTBIT_R_CMPBLK	0x0100		/**< リモート強制閉塞	(D08)		*/
#define CMD_CRDCTBIT_R_CHK		0x0020		/**< リモート診断		(D05)		*/
#define CMD_CRDCTBIT_R_NECHG	0x0008		/**< リモートNE切替	(D03)			*/
#define CMD_CRDCTBIT_R_BLKOFF	0x0004		/**< リモート閉塞解除	(D02)		*/
#define CMD_CRDCTBIT_R_BLK		0x0002		/**< リモート閉塞		(D01)		*/
#define CMD_CRDCTBIT_R_RST		0x0001		/**< リモートリセット	(D00)		*/

#define CMD_CRDCTBIT_VBIT	(\
			CMD_CRDCTBIT_L_BLKOFF	|\
			CMD_CRDCTBIT_L_BLK		|\
			CMD_CRDCTBIT_L_RST		|\
			CMD_CRDCTBIT_R_ALM		|\
			CMD_CRDCTBIT_R_ERR		|\
			CMD_CRDCTBIT_R_ERROFF	|\
			CMD_CRDCTBIT_R_CMPBLK	|\
			CMD_CRDCTBIT_R_CHK		|\
			CMD_CRDCTBIT_R_NECHG	|\
			CMD_CRDCTBIT_R_BLKOFF	|\
			CMD_CRDCTBIT_R_BLK		|\
			CMD_CRDCTBIT_R_RST		)		/**< 有効ビット					*/

/* 機能部固有制御項目 */
#define CMD_FNCCTBIT_MKSPC		0x0800		/**< メーカ固有	(D11)			*/
#define CMD_FNCCTBIT_VBIT	(\
			CMD_FNCCTBIT_MKSPC		)		/**< 有効ビット					*/

/* キャリア情報 */
#define CMD_CARIFBIT_CAR04		0x0800		/**< キャリア4	(D11)			*/
#define CMD_CARIFBIT_CAR03		0x0400		/**< キャリア3	(D10)			*/
#define CMD_CARIFBIT_CAR02		0x0200		/**< キャリア2	(D09)			*/
#define CMD_CARIFBIT_CAR01		0x0100		/**< キャリア1	(D08)			*/

#define CMD_CARIFBIT_VBIT	(\
			CMD_CARIFBIT_CAR04	|\
			CMD_CARIFBIT_CAR03	|\
			CMD_CARIFBIT_CAR02	|\
			CMD_CARIFBIT_CAR01	)			/**< 有効ビット					*/

/* パラメータチェック */
#define CMD_CNTCODEMAX			31			/**< 制御コード範囲値			*/
#define CMD_CODEMAX				65535		/**< 制御コード範囲値			*/
#define CMD_YEARMAX				0x9999		/**< 年最大値					*/
#define CMD_MONDAYMAX			0x9999		/**< 月日最大値					*/
#define CMD_HOURMAX				0x99		/**< 時最大値					*/
#define CMD_MINSECMAX			0x9999		/**< 分秒最大値					*/
#define CMD_CRDALM_INFNUMMAX	255			/**< 最大ALM情報数				*/
#define CMD_CRDERR_INFNUMMAX	255			/**< 最大ERR情報数				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CRDALM_INFNUMMAX_HIG	5107			/**< 最大ALM情報数				*/
#define CMD_CRDERR_INFNUMMAX_HIG	5107			/**< 最大ERR情報数				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CRDALM_INFMIN		1			/**< ALM情報最小値				*/

//3.5G-SRE
#define CMD_CRDERR_INFMIN		1			/**< ERR情報最小値				*/
//3.5G-SRE

#define CMD_CRDFB_INFMIN		1			/**< 機能部情報最小値			*/
#define CMD_CRDFB_INFMAX		28			/**< 機能部情報最大値			*/

/************************/
/* RE状態報告パラメータ */
/************************/
#define CMD_FANALM_MAXNUM		4			/**< FANALMビット最大数			*/
#define CMD_BARIALM_MAXNUM		4			/**< バリスタALMビット最大数	*/
#define CMD_BRKALM_MAXNUM		2			/**< ブレーカALMビット最大数	*/

#define CMD_FANALM_INF			0x1000		/**< FANALM情報報告値			*/
#define CMD_BARIALM_INF			0x0100		/**< バリスタALM情報報告値		*/
#define CMD_POWALM_INF			0x0200		/**< POWER ALM情報報告値		*/
#define CMD_BRETERMALM_INF		0x0400		/**< BRE-TERM ALM情報報告値		*/
#define CMD_PLLUNLCKALM_INF		0x0800		/**< PLL UNLOCK ALM情報報告値	*/
#define CMD_RFCLKDISCALM_INF	0x0F00		/**< RF CLK DISC ALM情報報告値	*/

/* パラメータチェック */
#define CMD_FANMIN				1			/**< FAN情報最小値				*/
#define CMD_MK_REMIN			1			/**< メーカ固有RE情報最小値		*/
#define CMD_NODATA				0			/**< テーブル初期値				*/

/* ======================================================================== */
/* BRE info indication register                */
/* ======================================================================== */
#define	CMD_REGADR_BREINF_DSP	0x90000210	/*	BRE付帯品情報表示			*/
#define	CMD_REGADR_BREINF_MASK	0x90000214	/*	BRE付帯品情報表示マスク		*/

#define	CMD_BRETERM_ALM_BIT		0x00008000			/* BRE-TERM CARD alarm	*/
#define	CMD_FAN_ALM_BIT			0x00000004			/* FAN alarm			*/

#define	CMD_REGVAL_BREINF_MASK_LRE	CMD_FAN_ALM_BIT

/******************************/
/* REカード制御要求パラメータ */
/******************************/
/* 制御 code for S3G */

#define CMD_CARDRST				1			/**< リセット  for S3G			*/
#define CMD_CARDBLK				2			/**< 閉塞	 for S3G			*/
#define CMD_NOBLK				3			/**< 閉塞解除	 for S3G		*/
#define CMD_NECHG				4			/**< N/E切替		 for S3G	*/
#define CMD_ENFBLK				5			/**< 強制閉塞	 for S3G		*/
#define CMD_CARDALM				6			/**< ALM for S3G				*/
#define CMD_CARDDIA				13			/**< card diagnosis				*/
#define CMD_2BR_SLP				19			/**< スリープ  */
#define CMD_BR0_SLP				20			/**< スリープ（0系） */
#define CMD_BR1_SLP				21			/**< スリープ（1系） */
#define CMD_UNSLP	    		22			/**< スリープ解除  */
#define CMD_BR2_SLP				24			/**< スリープ（2系）			*/
#define CMD_BR3_SLP				25			/**< スリープ（3系）			*/
#define CMD_BR4_SLP				26			/**< スリープ（4系）			*/
#define CMD_BR5_SLP				27			/**< スリープ（5系）			*/
#define CMD_BR6_SLP				28			/**< スリープ（6系）			*/
#define CMD_BR7_SLP				29			/**< スリープ（7系）			*/

/*****************************************/
/* 無変調キャリア出力ONOFF指定パラメータ */
/*****************************************/
/* 系指定 */
#define CMD_0SYSTEM				0			/**< 0系							*/
#define CMD_1SYSTEM				1			/**< 1系							*/

/* 送信電力 */
#define CMD_MIN_SNDPWR			350			/**< 3G送信電力最小値				*/
#define CMD_MAX_SNDPWR			630			/**< 3G送信電力最大値				*/

#define CMD_MIN_SNDPWR_S3G		458			/**< S3G送信電力最小値				*/
#define CMD_MAX_SNDPWR_S3G		660			/**< S3G送信電力最大値				*/

/****************************/
/* REリセット要求パラメータ	*/
/****************************/	
/* 対象指定	*/
#define CMD_RE					0			/**< RE							*/
#define CMD_LNKALL				1			/**< CPRIリンク配下				*/

/* RE共通部指定	*/
#define CMD_COM_CTIN			0			/**< 共通部含む					*/
#define CMD_COM_DONCARE			0			/**< Don't Care					*/
#define CMD_COM_NOCTIN			1			/**< 共通部含まない				*/

/************************************/
/* RE共通部リンク確立要求パラメータ */
/************************************/
/* 設定 */
#define CMD_ACTIVE			0				/**< Active						*/
#define CMD_NEGATIVE		1				/**< Negative					*/

/************************************/
/* REキャリア状態報告応答パラメータ */
/************************************/
/* キャリア構成状態	*/
#define CMD_NMLSND				0			/**< 正常送信					*/
#define CMD_ABNMLSND_0			1			/**< 0系送信異常				*/
#define CMD_ABNMLSND_1			2			/**< 1系送信異常				*/

/* キャリア状態		*/
#define CMD_CARSTBIT_USE		0x8000		/**< 使用中						*/
#define CMD_CARSTBIT_ALM		0x1000		/**< ALM						*/
#define CMD_CARSTBIT_ERR		0x0800		/**< ERR						*/
#define CMD_CARSTBIT_RSVUSE		0x0400		/**< 予備カード使用中			*/
#define CMD_CARSTBIT_VLD		0x0200		/**< 有効						*/

#define CMD_CARSTBIT_VBIT	(\
			CMD_CARSTBIT_USE	|\
			CMD_CARSTBIT_ALM	|\
			CMD_CARSTBIT_ERR	|\
			CMD_CARSTBIT_RSVUSE	|\
			CMD_CARSTBIT_VLD	)			/**< 有効ビット					*/

/* パラメータチェック */
#define CMD_CARALM_INFNUMMAX	255			/**< 最大キャリアALM情報数		*/
#define CMD_CARERR_INFNUMMAX	255			/**< 最大キャリアERR情報数		*/
#define CMD_CARALM_INFMIN		221			/**< キャリアALM最小値			*/
#define CMD_CARALM_INFMAX		255			/**< キャリアALM最大値			*/
#define CMD_CARERR_INFMIN		221			/**< キャリアERR最小値			*/
#define CMD_CARERR_INFMAX		255			/**< キャリアERR最大値			*/
#define CMD_CARLMAX				2			/**< キャリア最大構成数			*/
#define CMD_CARCRDMAX			6			/**< キャリアカード最大値		*/

/* ======================================================================== */
/*						  		メーカ固有情報数							*/
/* ======================================================================== */
#define CMD_ALM_NUM				128			/**< ALM情報数					*/
#define CMD_ERR_NUM				16			/**< ERR情報数					*/


/************************************/
/* RE装置構成情報報告応答パラメータ */
/************************************/
/* CPRIリンク装置構成情報 			*/
/************************************/
/* BTS形番 */
#define CMD_BTS_NO				3001		/**< BTS型番					*/
/* SRE対応 - Start */
#define CMD_BTS_SRE_NO			3201		/**< BTS型番					*/
/* SRE対応 - End */

/* RE種別 */
#define CMD_REKIND_BRE			1			/**< AMP (BRE)					*/
#define CMD_REKIND_RRELRE		2			/**< OF-TRX (RRE/LRE)			*/

/*FPGA種別 */
#define CMD_FPGATYPE_CPRI		1			/**< CPRI-FPGA					*/
#define CMD_FPGATYPE_APD		2			/**< APD-FPGA					*/

/* 電源種別 */
#define CMD_POWERKIND_DC		0			/**< DC							*/
#define CMD_POWERKIND_AC1		1			/**< AC100V						*/
#define CMD_POWERKIND_AC2		2			/**< AC200V						*/
#define CMD_POWERKIND_AC1_2		3			/**< AC100V/AC200V				*/

/* パワークラス単位 */
#define CMD_POWERUNIT_W			0			/**< W							*/
#define CMD_POWERUNIT_MW		1			/**< mW							*/
#define CMD_POWERUNIT_MICROW	2			/**< μW						*/

/* 対応周波数帯 */
#define CMD_FRQBAND_2			0			/**< 2GHz帯						*/
#define CMD_FRQBAND_8			1			/**< 800MHz帯					*/
#define CMD_FRQBAND_17			2			/**< 1.7GHz帯					*/
#define CMD_FRQBAND_2_8			3			/**< 2GHz帯/800mHz帯			*/
#define CMD_FRQBAND_2_17		4			/**< 2GHz帯/1.7GHz帯			*/
#define CMD_FRQBAND_8_17		5			/**< 800MHz帯/1.7GHz帯			*/
#define CMD_FRQBAND_2_8_17		6			/**< 2GHz帯/800MHz帯/1.7GHz帯	*/
#define CMD_FRQBAND_15			7			/**< 1.5GHz帯					*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_FRQBAND_2_LTE       0x0001		/**< 2GHz帯						*/
#define CMD_FRQBAND_15_LTE      0x0002		/**< 1.5GHz帯					*/
#define CMD_FRQBAND_800_LTE     0x0004		/**< 800MHz帯					*/
#define CMD_FRQBAND_700_LTE     0x0008		/**< 700MHz帯					*/
#define CMD_FRQBAND_17_LTE      0x0010		/**< 1.7GHz帯					*/
#define CMD_FRQBAND_17L_LTE     0x0020		/**< 1.7GHz LowBand帯			*/
#define CMD_FRQBAND_35_LTE      0x0040		/**< 3.5GHz帯					*/
#define CMD_FRQBAND_34_LTE      0x0080		/**< 3.4GHz帯					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 800MHz帯フィルタタイプ */
#define CMD_800BAND_FLT_A		0			/**< Type-A						*/
#define CMD_800BAND_FLT_B		1			/**< Type-B						*/
/*#define CMD_800BAND_FLT_AB		2	*/	/**< Type-B		TBD				*/

/* RE側TRX-INFタイプ情報 */
#define CMD_13DB				0			/**< 13dB						*/
#define CMD_21DB				1			/**< 21dB						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_5_14DB				2			/**< 5~14dB						*/
#define CMD_13_21DB				3			/**< 13~21dB					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* パラメータチェック */
#define CMD_MAKERMIN			0x41	/**< メーカ名最小					*/
#define CMD_MAKERMAX			0x5a	/**< メーカ名最大					*/
#define CMD_EQPMIN				1		/**< 装置号番最小					*/
#define CMD_EQPMAX				4		/**< 装置号番最大					*/
#define CMD_POWERMAX			3		/**< 電源種別最大					*/
#define CMD_PUNITMAX			2		/**< パワークラス単位最大値			*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_PUNITMIN			1		/**< パワークラス単位最小値			*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_BANDMAX				7		/**< 対応周波数帯最大値				*/
#define CMD_800FILMAX			1		/**< 800MHz帯フィルタタイプ以上		*/
#define CMD_2GCARLMAX			4		/**< 2GHz帯キャリア数最大値			*/
#define CMD_800CARLMAX			4		/**< 800MHz帯キャリア数最大値		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_800CARLMAX_3G		3		/**< 800MHz帯キャリア数最大値(3G)	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_17GCARLMAX			4		/**< 1.7GHz帯キャリア数最大値		*/
#define CMD_15GCARLMAX			4		/**< 1.5GHz帯キャリア数最大値		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_35GCARLMAX			2		/**< 3.5GHz帯キャリア数最大値		*/
#define CMD_35GCARLMIN			1		/**< 3.5GHz帯キャリア数最小値		*/
#define CMD_34GCARLMAX			2		/**< 3.4GHz帯キャリア数最大値		*/
#define CMD_34GCARLMIN			1		/**< 3.4GHz帯キャリア数最小値		*/
#define CMD_EQP1_3G				1		/**< 装置号番最小					*/
#define CMD_EQP2_3G				2		/**< 装置号番最大					*/
#define CMD_SYSBANDMIN			1		/**< 最大周波数帯域幅 min value		*/
#define CMD_SYSBANDMAX			20		/**< 最大周波数帯域幅 max value		*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_EQUINF_MAX			64		/**< 装置構成情報数					*/

/********************************/
/* RE障害ログ取得要求パラメータ */
/********************************/
/* RE共通部/REセクタ部指定 */
#define CMD_RE_COM				0			/**< RE共通部					*/
#define CMD_RE_SEC				1			/**< REセクタ部					*/

#define CMD_FILENAME_MAX		256			/**< RE障害ログファイル名最大長	*/

/********************************/
/* ファイル転送通知パラメータ */
/********************************/
#define CMD_FILEDAT_MAX			10240		/**< ファイル転送サイズ最大値	*/

/************************************/
/* 外部装置データ送信要求パラメータ */
/************************************/
#define CMD_DAT_EXTDATSZ		2048		/**< 送信データサイズ最大値		*/

/************************************/
/* 外部装置データ受信要求パラメータ */
/************************************/
/* 外部装置番号 */
#define CMD_OARA_INF_1			1			/**< OA-RA-INF1					*/
#define CMD_OARA_INF_2			2			/**< OA-RA-INF2					*/
#define CMD_MTT_INF				3			/**< MTT-INF					*/
#define CMD_TILT_INF			4			/**< TILT-INF					*/


#define CMD_OARA_INF_S3G		1			/**< OA-RA-INF	 S3G			*/
#define CMD_TILT_INF_S3G		2			/**< TILT-INF	 S3G			*/

/********************************/
/* MTアドレス設定要求パラメータ */
/********************************/
/* 対象MT */
#define CMD_FLD_MT				0			/**< FLD-MT						*/
#define CMD_SV_MT				1			/**< SV-MT						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_MT_MAX              2
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* IPアドレス */
#define CMD_IPADD_1			192			/**< IPアドレス#1					*/
#define CMD_IPADD_2			168			/**< IPアドレス#2					*/
#define CMD_IPADD_3			1			/**< IPアドレス#3					*/
#define CMD_IPADD_4			129			/**< IPアドレス#4					*/

/************************************/
/* FLD-MT/SV-MTデータ転送パラメータ */
/************************************/
#define CMD_FLDCMD_MXLEN			71680	/**< FLD-MT FLD-MTデータコマンド転送最大サイズ	ZTX_ZTX (66560)    */
#define CMD_FLDCMD_BLKLEN			10240	/**< FLD-MTデータコマンド転送最大ブロックサイズ	*/
#define CMD_FLDFIL_MXLEN			4096	/**< FLD-MT data file max length 	*/
#define CMD_SMASCDAT_MXLEN_3G		4096	/**< SV-MT data max length (ascii data) for 3G */
#define CMD_SMBINDAT_MXLEN_3G		2048	/**< SV-MT data max length (binary data)	 for 3G 	*/
#define CMD_SMASCDAT_MXLEN_S3G		20480	/**< SV-MT data max length (ascii data) for S3G */
#define CMD_SMBINDAT_MXLEN_S3G		10240	/**< SV-MT data max length (binary data)	 for S3G 	*/
#define CMD_FLDCMD_MAXBLK			7		/**< FLD-MTデータコマンド転送最大ブロック数	*/

/********************************/
/* カード情報報告応答パラメータ */
/********************************/
/* 上りリソース数 */
#define CMD_TRA_UL_RSCNUM_S3G		1			/**< MDIF上りリソース数(固定)		*/
/* 下りリソース数 */
#define CMD_TRA_DL_RSCNUM_S3G		1			/**< MDIF下りリソース数(固定)		*/
/* 基本監視項目 */
#define CMD_MDIF_BASEITEM			0x37E6		/**< TRA基本監視項目(固定)		*/ /*MB BE 0x03E6 */
//TDD-SRE
#define CMD_MDIF_BASEITEM_S3G		0x07E6		/**< TRA基本監視項目(固定) FOR S3G		*/ 
#define CMD_MDIF_BASEITEM_LRE		0x07E6		/**< TRA基本監視項目(固定)		*/ /*MB BE 0x03E6 */

/* 拡張監視項目 */
#define CMD_TRA_EXTITEM				0x8002		/**< TRA拡張監視項目(固定)	for 3G	*/
#define CMD_TRA_EXTITEM_S3G			0x0076		/**< TRA拡張監視項目(固定)	for S3G	*/
/* 制御項目 */
#define CMD_TRA_CNTITEM				0xEF27		/**< TRA制御項目(固定)	for 3G		*/
#define CMD_TRA_CNTITEM_S3G			0x7077		/**< TRA制御項目(固定)	for S3G		*/
/* 拡張制御項目 */
#define CMD_TRA_EXT_CNTITEM_S3G		0x001E		/**< TRA拡張制御項目(固定)	for S3G	*/
/* 機能部固有情報 */
#define CMD_FUNCITEM				0x00E0		/**< 機能部固有情報				*/
#define CMD_FUNCITEM_LRE			0x0000		/**< 機能部固有情報				*/


/***********************************/
/* TOFFSET値/上り/下り装置内遅延値 */
/***********************************/

/* TOFFSET値は全装置種別共通値 */
#define CMD_S3GCONN_TOFFSET_NUM_35G_SRE		400			/**< S3G 単独接続のTOFFSET値 for TD-SRE 3.5G */

/* RRE 装置 */
/* 3G単独接続 */
#define CMD_S3GCONN_UL_NUM_35G_SRE			3300		/**< S3G 単独接続の上り装置内遅延量 for TD-SRE 3.5G */


/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_S3GCONN_DL_NUM_35G_SRE			3300		/**< S3G 単独接続の下り装置内遅延量 for TD-SRE 3.5G */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 3Gカスケード接続 */
#define CMD_S3GCONN_UL_NUM_35G_RRE			3302		/**< S3G 単独接続の上り装置内遅延量 for TD-RRE_ZYNQ 3.5G */

#define CMD_S3GCONN_DL_NUM_35G_RRE			3297		/**< S3G 単独接続の下り装置内遅延量 for TD-RRE_ZYNQ 3.5G */


/*****************************/
/* 応答結果全般パラメータ	 */
/*****************************/
#define CMD_NML					0			/**< 正常						*/
#define CMD_INFRPT_NG			1			/**< 情報報告NG					*/
#define CMD_SET_NG				1			/**< 設定NG						*/
#define CMD_CHKSUM_NG			1			/**< チェックサムNG				*/
#define CMD_FRM_NOFILE			1			/**< ファームウェアファイルなし	*/
#define CMD_SNDDAT_NUM_ABNML	1			/**< 送信データ順序番号異常		*/
#define CMD_SNDDATSEQNG			1			/**< 送信データ順序番号異常		*/
#define CMD_RCVDATSEQNG			1			/**< 受信データ順序番号異常		*/
#define CMD_RPT_NG				2			/**< 報告NG						*/
#define CMD_TIMOUT				2			/**< タイムアウト				*/
#define CMD_INF_NG				3			/**< 情報NG						*/
#define CMD_BUFF_FUL			19			/**< バッファフル				*/
#define CMD_HARD_ABNML_RE		41			/**< ハードウェア異常(RE)		*/
#define CMD_OTHR_ERR_RE			42			/**< その他エラー(RE)			*/
#define CMD_CNT_NG_RE			43			/**< 制御NG(RE)					*/
#define CMD_READ_NG_RE			43			/**< 読み出しNG(RE)				*/
#define CMD_REL_NG_RE			43			/**< 解放NG(RE)					*/
#define CMD_DIA_NG_RE			43			/**< 診断NG(RE)					*/
#define CMD_KIND_NG_RE			44			/**< 対象指定NG(RE)				*/
#define CMD_SET_NEGATIVE		45			/**< Negative設定				*/
#define CMD_BUFF_FUL_RE			46			/**< バッファフル(RE)			*/
#define CMD_INSLEEP_RE			47			/**< スリープ制御中(RE)		*/
#define CMD_HARD_ABNML_RE_S3G	50			/**< ハードウェア異常(RE)(for LTE) */
#define CMD_HDL_NG				51			/**< 処理NG（RE） */	
#define CMD_LOG_NG				52			/**< 障害ログ未対応				*/
#define CMD_KIND_NG_RE_S3G		52			/**< 対象指定NG(RE)(for LTE)			*/
#define CMD_SET_NEGATIVE_S3G	53			/**< Negative設定(for LTE)				*/
#define CMD_BUFF_FUL_RE_S3G		54			/**< バッファフル(RE)(for LTE)			*/
#define CMD_INSLEEP_RE_S3G		55			/**< スリープ制御中(RE)(for LTE)		*/
#define CMD_FUNCSET_ABNML_RE	56			/**< 機能部設定異常(RE)			*/
#define CMD_FRM_NOFILE_S3G		60			/**< ファームウェアファイルなし(for LTE) */
#define CMD_CHKSUM_NG_S3G		60			/**< チェックサムNG(for LTE) 			*/
#define CMD_BUFF_FUL_S3G		60			/**< バッファフル(REC)(for LTE)			*/
#define CMD_SNDDATSEQNG_S3G		60			/**< 送信データ順序番号異常(for LTE)	*/
#define CMD_RCVDATSEQNG_S3G		60			/**< 受信データ順序番号異常(for LTE)	*/
#define CMD_LOG_NG_S3G			60			/**< 障害ログ未対応(for LTE)			*/
#define CMD_FUNCSET_ABNML_RE_S3G	60		/**< 機能部設定異常(RE)(for LTE)		*/
#define CMD_TIMOUT_S3G				61		/**< タイムアウト(for LTE)				*/
#define CMD_READ_NG_RE_S3G			61		/**< 読み出しNG(RE)(for LTE)			*/
#define CMD_DIA_NG_RE_S3G			61		/**< 診断NG(RE)(for LTE)				*/
#define CMD_INF_NG_S3G				62		/**< 情報NG(for LTE) 					*/
#define CMD_AISG_HDL2_NG		221			/**< 送信データ(RETAP)内のパラメータ範囲異常 (旧版対応)*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_MTCNT_DISABLE_LTE	101			/**< MT接続不可(LTE)				    */
#define CMD_REC_RSP_NG_LTE		102			/**< REC応答NG							*/
#define CMD_FLD_CMDRCV_NG_LTE	103			/**< FLD-MTデータコマンド転送受信不可状態	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_KIND_NG_221			221			/**< 対象指定NG(221)						*/
#define CMD_CARD_NOINS_221		221			/**< 指定カード未実装(221)				*/
#define CMD_MAKER_RENG_MIN		221			/**< メーカ固有(RE)最小値(221)			*/
#define CMD_FMRXDISABLE			221			/**< FLD-MT信号受信不可状態				*/
#define CMD_SMRXDISABLE			221			/**< SV-MT信号受信不可状態				*/
#define CMD_SNDTRGNG			221			/**< 送信系統未対応  					*/
#define CMD_SVCNT_TO			221			/**< SVCNT T.O.							*/
#define CMD_FLDCMDRCV_NG		221			/**< NG in FLD-MT command receiving 	*/
#define CMD_FLDFILRCV_NG		221			/**< NG in FLD-MT file receiving		*/
#define CMD_SVDATRCV_NG			221			/**< NG in SV-MT data receiving			*/
#define CMD_REFW_NG				221			/**< REファームウェアファイル異常		*/
#define CMD_MAKER_RENG			221			/**< メーカ固有(RE)(221)				*/
#define CMD_EXSND_NG			221			/**< その他エラー(EXT DATA SNDING)		*/
#define CMD_NO_PORT				221			/**< PORT-INF機能部無し					*/
#define CMD_AISG_HDL_NG			222			/**< 送信データ(RETAP)内のパラメータ範囲異常*/
#define CMD_NO_FLD				222			/**< FLD未実装、ALM						*/
#define CMD_DEV_NUM_NG			222			/**< 指定装置番号NG						*/
#define CMD_S3G_BW_NG			222			/**< S3G帯域幅がシステムパラメータより大き */
#define CMD_3G_CARNO_NG			223			/**< 3G carrier no is not in syspara	*/
#define CMD_MTDAT_NOSYSPARA		223			/**< システムパラメータ対応無し			*/
#define CMD_MTREL_NOSYSPARA		222			/**< システムパラメータ対応無し			*/
#define CMD_DAT_RNG_NG			223			/**< データサイズ範囲外					*/
#define CMD_FLD_TO				223			/**< FLD異常、MDE-Bリセット中			*/
#define CMD_SV_TO				223			/**< SV異常								*/
#define CMD_SVMTRLS_NG			223			/**< SV-MT接続解放応答NG				*/
#define CMD_EXT_DEVNO_NG		223			/**< 外部装置番号対象外					*/
#define CMD_TRXSET_3G			223			/**< TRX設定(3G)受信済み				*/
#define CMD_MULTI_FLDMT			224			/**< 他のFLD-MTと接続中					*/
#define CMD_MULTI_SVMT			224			/**< 他のSV-MTと接続中					*/
#define CMD_MAKER_CODE			224			/**< メーカ固有(RE) error				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_FLD_FILSND_NG		225			/**< FLD-MTデータファイル転送送信不可状態	*/
#define CMD_FLD_FILSND_RE_NG	224			/**< 全配下RE応答NG	*/
#define CMD_SYSP_STA_NG			224			/**< 224：状態異常	*/
#define CMD_SYSP_PARA_INVALID	225			/**< 225：パラメータ未対応	*/
#define CMD_REC_RES_NG			232			/**< REC応答NG							*/
#define CMD_FLD_CMDRCV_NG		233			/**< FLD-MTデータコマンド転送受信不可状態		*/
#define CMD_FLD_CMDRCV_NG_LTE	103			/**< FLD-MTデータコマンド転送受信不可状態(LTE)	*/
#define	CMD_SVMT_CMDRCV_NG		233			/**< 受信不可状態						*/
#define CMD_SVMT_CMDRCV_NG_LTE	103			/**< 受信不可状態(LTE)					*/
#define CMD_MAKER_RENG_MAX		255			/**< メーカ固有(RE)最大値(255)			*/
#define CMD_MTCNT_DISABLE		231			/**< MT接続不可						    */
#define CMD_RECRS_NG			232			/**< REC応答NG							*/
#define CMD_OTH_ERR				235			/**<その他エラー						*/
#define CMD_ALLRE_NG			223			/**< 全配下RE応答NG						*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_MAKER_RENG_MAX		255			/**< メーカ固有(RE)最大値(255)			*/

/* CPRI-MT FB number */
#define CMD_FBNO_CPRI_SIM 			0x0a010000  /* simulator    */
#define CMD_FBNO_CPRI_MON			0x0a020000  /* monitor      */
#define CMD_FBNO_REMT				0x0a030000  /* monitor      */

/****************************************************************************/
/*	- No.2 -																*/
/*				CPRI信号種別/データ部レングス定義							*/
/*																			*/
/****************************************************************************/

	
/** @name CPRI signal ID definition */
/* @{ */

/* ======================================================================== */
/* CPRI信号種別定義 														*/
/* ======================================================================== */
#define CMD_CPRID_HCREQ			0x0110	/**< ヘルスチェック要求				*/
#define CMD_CPRID_HCRES			0x0120	/**< ヘルスチェック応答				*/
#define CMD_CPRID_RERSTREQ		0x0210	/**< REリセット要求					*/
/* 2021/01/21 M&C)Tri.hn Add based on No.29 (seq 3-2) - add start */
#define CMD_CPRID_RERSTRES		0x0220	/**< REリセット応答					*/
/* 2021/01/21 M&C)Tri.hn Add based on No.29 (seq 3-2) - add end */
#define CMD_CPRID_RETIMSETREQ	0x0310	/**< RE時刻設定要求					*/
#define CMD_CPRID_LINKSTSREQ	0x1110	/**< RE共通部リンク確立要求			*/
#define CMD_CPRID_LINKSTSRES	0x1120	/**< RE共通部リンク確立応答			*/
#define CMD_CPRID_FIRMFILEREQ	0x2110	/**< ファームウェアファイル報告要求	*/
#define CMD_CPRID_FIRMFILERES	0x2120	/**< ファームウェアファイル報告応答	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CPRID_FIRMFILERES2	0x2130	/**< ファームウェアファイル報告応答2*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CPRID_FILEINFREQ	0x2210	/**< ファイル情報報告通知				*/
#define CMD_CPRID_FILEINFRES	0x2220	/**< ファイル情報報告応答				*/
#define CMD_CPRID_FILEDATSND	0x2300	/**< ファイルデータ送信				*/
#define CMD_CPRID_FILSNDFINNTC	0x2310	/**< ファイルデータ送信完了報告通知	*/
#define CMD_CPRID_FILSNDFINRES	0x2320	/**< ファイルデータ送信完了報告応答	*/
#define CMD_CPRID_UFIRMFILEREQ	0x2410	/**< 運用中ファームウェアファイル報告要求	*/
#define CMD_CPRID_UFIRMFILERES	0x2420	/**< 運用中ファームウェアファイル報告応答	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CPRID_UFIRMFILERES2	0x2430	/**< 運用中ファームウェアファイル報告応答2	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CPRID_UFILEINFREQ	0x2510	/**< 運用中ファイル情報報告通知				*/
#define CMD_CPRID_UFILEINFRES	0x2520	/**< 運用中ファイル情報報告応答				*/
#define CMD_CPRID_UFILEDATSND	0x2600	/**< 運用中ファイルデータ送信				*/
#define CMD_CPRID_UFILSNDFINNTC	0x2610	/**< 運用中ファイルデータ送信完了報告通知	*/
#define CMD_CPRID_UFILSNDFINRES	0x2620	/**< 運用中ファイルデータ送信完了報告応答	*/
#define CMD_CPRID_TRXSETREQ		0x4110	/**< TRX設定要求						*/
#define CMD_CPRID_TRXSETRES		0x4120	/**< TRX設定応答						*/
#define CMD_CPRID_TRXRELREQ		0x4210	/**< TRX解放要求						*/
#define CMD_CPRID_TRXRELRES		0x4220	/**< TRX解放応答						*/
#define CMD_CPRID_TOFFSETINFREQ	0x4310	/**< Toffset値報告要求				*/
#define CMD_CPRID_TOFFSETINFRES	0x4320	/**< Toffset値報告応答				*/
#define CMD_CPRID_CARONOFFREQ	0x4410	/**< 無変調キャリアON/OFF指定要求		*/
#define CMD_CPRID_CARONOFFRES	0x4420	/**< 無変調キャリアON/OFF指定応答		*/
#define CMD_CPRID_EQPSTRREQ		0x5110	/**< RE装置構成情報報告要求			*/
#define CMD_CPRID_EQPSTRRES		0x5120	/**< RE装置構成情報報告応答			*/
#define CMD_CPRID_SLOTINFREQ	0x5210	/**< REスロット情報報告要求			*/
#define CMD_CPRID_SLOTINFRES	0x5220	/**< REスロット情報報告応答			*/
#define CMD_CPRID_RESTSREQ		0x5310	/**< RE状態報告要求					*/
#define CMD_CPRID_RESTSRES		0x5320	/**< RE状態報告応答					*/
#define CMD_CPRID_CARDINFREQ	0x5410	/**< REカード情報報告要求				*/
#define CMD_CPRID_CARDINFRES	0x5420	/**< REカード情報報告応答				*/
#define CMD_CPRID_REVERREQ		0x5510	/**< REバージョン報告要求				*/
#define CMD_CPRID_REVERRES		0x5520	/**< REバージョン報告応答				*/
#define CMD_CPRID_RECARDCNTREQ	0x5610	/**< REカード制御要求					*/
#define CMD_CPRID_RECARDCNTRES	0x5620	/**< REカード制御応答					*/
#define CMD_CPRID_CARDSTSREQ	0x5710	/**< REカード状態報告要求				*/
#define CMD_CPRID_CARDSTSRES	0x5720	/**< REカード状態報告応答				*/
#define CMD_CPRID_CARLSTSREQ	0x5810	/**< REキャリア状態報告要求			*/
#define CMD_CPRID_CARLSTSRES	0x5820	/**< REキャリア状態報告応答			*/
#define CMD_CPRID_REPORTCNTREQ	0x5910	/**< RE PORT制御要求					*/
#define CMD_CPRID_REPORTCNTRES	0x5920	/**< RE PORT制御応答					*/
#define CMD_CPRID_REPORTSTSREQ	0x5A10	/**< RE PORT状態報告要求				*/
#define CMD_CPRID_REPORTSTSRES	0x5A20	/**< RE PORT状態報告応答				*/
#define CMD_CPRID_RECRDDIAREQ	0x5B10	/**< REカード診断要求					*/
#define CMD_CPRID_RECRDDIARES	0x5B20	/**< REカード診断報告					*/
#define CMD_CPRID_REOBSLOGREQ	0x5C10	/**< RE障害ログ取得要求				*/
#define CMD_CPRID_REOBSLOGRES	0x5C20	/**< RE障害ログ取得応答				*/
#define CMD_CPRID_REOBLOGSTPREQ	0x5D10	/**< RE障害ログ取得中止要求			*/
#define CMD_CPRID_REOBLOGSTPRES	0x5D20	/**< RE障害ログ取得中止応答			*/
#define CMD_CPRID_EXTDATSNDREQ	0x9110	/**< 外部装置データ送信要求			*/
#define CMD_CPRID_EXTDATSNDRES	0x9120	/**< 外部装置データ送信応答			*/
#define CMD_CPRID_EXTDATRCVREQ	0x9130	/**< 外部装置データ受信要求			*/
#define CMD_CPRID_EXTDATRCVRES	0x9140	/**< 外部装置データ受信応答			*/
#define CMD_CPRID_MTADDSETREQ	0x9210	/**< MT address setting request for 3G	*/
#define CMD_CPRID_MTADDSETRES	0x9220	/**< MTアドレス設定応答				*/
#define CMD_CPRID_FLDMTCNCREQ	0x9310	/**< FLD-MT接続要求					*/
#define CMD_CPRID_FLDMTCNCRES	0x9320	/**< FLD-MT接続応答					*/
#define CMD_CPRID_FLDCOMSNDREQ	0x9370	/**< FLD-MTデータコマンド転送送信要求	*/
#define CMD_CPRID_FLDCOMSNDRES	0x9380	/**< FLD-MTデータコマンド転送送信応答	*/
#define CMD_CPRID_FLDCOMRCVREQ	0x9390	/**< FLD-MTデータコマンド転送受信要求	*/
#define CMD_CPRID_FLDCOMRCVRES	0x93A0	/**< FLD-MTデータコマンド転送受信応答	*/
#define CMD_CPRID_FLDFILSNDREQ	0x93B0	/**< FLD-MTデータファイル転送送信要求	*/
#define CMD_CPRID_FLDFILSNDRES	0x93C0	/**< FLD-MTデータファイル転送送信応答	*/
#define CMD_CPRID_FLDFILRCVREQ	0x93D0	/**< FLD-MTデータファイル転送受信要求	*/
#define CMD_CPRID_FLDFILRCVRES	0x93E0	/**< FLD-MTデータファイル転送受信応答	*/
#define CMD_CPRID_FLMTCNCRELREQ	0x9330	/**< FLD-MT接続解放要求				*/
#define CMD_CPRID_FLMTCNCRELRES	0x9340	/**< FLD-MT接続解放応答				*/
#define CMD_CPRID_FLMTCMPRELREQ	0x9350	/**< FLD-MT接続強制解放要求			*/
#define CMD_CPRID_FLMTCMPRELRES	0x9360	/**< FLD-MT接続強制解放応答			*/
#define CMD_CPRID_SVMTCNTREQ	0x9410	/**< SV-MT接続要求					*/
#define CMD_CPRID_SVMTCNTRES	0x9420	/**< SV-MT接続応答					*/
#define CMD_CPRID_SVMTDATSNDREQ	0x9470	/**< SV-MTデータ送信要求				*/
#define CMD_CPRID_SVMTDATSNDRES	0x9480	/**< SV-MTデータ送信応答				*/
#define CMD_CPRID_SVMTDATRCVREQ	0x9490	/**< SV-MTデータ受信要求				*/
#define CMD_CPRID_SVMTDATRCVRES	0x94A0	/**< SV-MTデータ受信応答				*/
#define CMD_CPRID_SVMTCNTRELREQ	0x9430	/**< SV-MT接続解放要求				*/
#define CMD_CPRID_SVMTCNTRELRES	0x9440	/**< SV-MT接続解放応答				*/
#define CMD_CPRID_SVMTFORRELREQ	0x9450	/**< SV-MT接続強制解放要求			*/
#define CMD_CPRID_SVMTFORRELRES	0x9460	/**< SV-MT接続強制解放応答			*/
#define CMD_CPRID_MKMTDATSND	0x9500	/**< メーカMTデータ送信				*/
#define CMD_CPRID_MKMTCNTREQ	0x9510	/**< メーカMT接続要求					*/
#define CMD_CPRID_MKMTCNTRES	0x9520	/**< メーカMT接続応答					*/
#define CMD_CPRID_MKMTCNTRELREQ	0x9530	/**< メーカMT接続解放要求				*/
#define CMD_CPRID_MKMTCNTRELRES	0x9540	/**< メーカMT接続解放応答				*/
#define CMD_CPRID_MKMTFORRELREQ	0x9550	/**< メーカMT接続強制解放要求			*/
#define CMD_CPRID_MKMTFORRELRES	0x9560	/**< メーカMT接続強制解放応答			*/
#define CMD_CPRID_FTPPORTSETREQ 0x9610  /**< FTP データ送信用PORT 番号設定要求	*/
#define CMD_CPRID_FTPPORTSETRES 0x9620  /**< FTP データ送信用PORT 番号設定応答	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CPRID_MTADDSETTOREQ	0x9990	/**< MTアドレス設定確認タイマ」タイムアウト(REC) */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* ======================================================================== */
/* CPRI信号データ部レングス定義												*/
/* ======================================================================== */
#define CMD_CPRLN_HCREQ				/**< ヘルスチェック要求				*/
#define CMD_CPRLN_HCRES				/**< ヘルスチェック応答				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CPRLN_RERSTREQ		6	/**< REリセット要求					*/
#define CMD_CPRLN_RETIMSETREQ	10	/**< RE時刻設定要求					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CPRLN_LINKSTSREQ		/**< RE共通部リンク確立要求			*/
#define CMD_CPRLN_LINKSTSRES		/**< RE共通部リンク確立応答			*/
#define CMD_CPRLN_FIRMFILEREQ		/**< ファームウェアファイル報告要求	*/
#define CMD_CPRLN_FIRMFILERES		/**< ファームウェアファイル報告応答	*/
#define CMD_CPRLN_FILEINFREQ		/**< ファイル情報報告通知			*/
#define CMD_CPRLN_FILEINFRES		/**< ファイル情報報告応答			*/
#define CMD_CPRLN_FILEDATSND		/**< ファイルデータ送信				*/
#define CMD_CPRLN_FILSNDFINNTC		/**< ファイルデータ送信完了報告通知	*/
#define CMD_CPRLN_FILSNDFINRES		/**< ファイルデータ送信完了報告応答	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CPRLN_TRXSETREQ		  10/**< TRX設定要求					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_CPRLN_TRXSETRES			/**< TRX設定応答					*/
#define CMD_CPRLN_TRXRELREQ		  4 /**< TRX解放要求					*/
#define CMD_CPRLN_TRXRELRES			/**< TRX解放応答					*/
#define CMD_CPRLN_TOFFSETINFREQ	  2	/**< Toffset値報告要求				*/
#define CMD_CPRLN_TOFFSETINFRES		/**< Toffset値報告応答				*/
#define CMD_CPRLN_CARONOFFREQ		/**< 無変調キャリアON/OFF指定要求	*/
#define CMD_CPRLN_CARONOFFRES		/**< 無変調キャリアON/OFF指定応答	*/
#define CMD_CPRLN_EQPSTRREQ		  2	/**< RE装置構成情報報告要求			*/
#define CMD_CPRLN_EQPSTRRES			/**< RE装置構成情報報告応答			*/
#define CMD_CPRLN_SLOTINFREQ	  2	/**< REスロット情報報告要求			*/
#define CMD_CPRLN_SLOTINFRES		/**< REスロット情報報告応答			*/
#define CMD_CPRLN_RESTSREQ		  2	/**< RE状態報告要求					*/
#define CMD_CPRLN_RESTSRES			/**< RE状態報告応答					*/
#define CMD_CPRLN_CARDINFREQ		/**< REカード情報報告要求			*/
#define CMD_CPRLN_CARDINFRES		/**< REカード情報報告応答			*/
#define CMD_CPRLN_REVERREQ			/**< REバージョン報告要求			*/
#define CMD_CPRLN_REVERRES			/**< REバージョン報告応答			*/
#define CMD_CPRLN_RECARDCNTREQ		/**< REカード制御要求				*/
#define CMD_CPRLN_RECARDCNTRES		/**< REカード制御応答				*/
#define CMD_CPRLN_CARDSTSREQ	  4	/**< REカード状態報告要求			*/
#define CMD_CPRLN_CARDSTSRES		/**< REカード状態報告応答			*/
#define CMD_CPRLN_CARLSTSREQ	  2	/**< REキャリア状態報告要求			*/
#define CMD_CPRLN_CARLSTSRES		/**< REキャリア状態報告応答			*/
#define CMD_CPRLN_REPORTCNTREQ		/**< RE PORT制御要求				*/
#define CMD_CPRLN_REPORTCNTRES		/**< RE PORT制御応答				*/
#define CMD_CPRLN_REPORTSTSREQ	  2	/**< RE PORT状態報告要求			*/
#define CMD_CPRLN_REPORTSTSRES		/**< RE PORT状態報告応答			*/
#define CMD_CPRLN_RECRDDIAREQ		/**< REカード診断要求				*/
#define CMD_CPRLN_RECRDDIARES		/**< REカード診断報告				*/
#define CMD_CPRLN_REOBSLOGREQ		/**< RE障害ログ取得要求				*/
#define CMD_CPRLN_REOBSLOGRES		/**< RE障害ログ取得応答				*/
#define CMD_CPRLN_REOBLOGSTPREQ		/**< RE障害ログ取得中止要求			*/
#define CMD_CPRLN_REOBLOGSTPRES		/**< RE障害ログ取得中止応答			*/
#define CMD_CPRLN_EXTDATSNDREQ		/**< 外部装置データ送信要求			*/
#define CMD_CPRLN_EXTDATSNDRES		/**< 外部装置データ送信応答			*/
#define CMD_CPRLN_EXTDATRCVREQ		/**< 外部装置データ受信要求			*/
#define CMD_CPRLN_EXTDATRCVRES		/**< 外部装置データ受信応答			*/
#define CMD_CPRLN_MTADDSETREQ	 18	/**< MTアドレス設定要求				*/
#define CMD_CPRLN_MTADDSETRES		/**< MTアドレス設定応答				*/
#define CMD_CPRLN_FLDMTCNCREQ		/**< FLD-MT接続要求					*/
#define CMD_CPRLN_FLDMTCNCRES		/**< FLD-MT接続応答					*/
#define CMD_CPRLN_FLDCOMSNDREQ		/**< FLD-MTデータコマンド転送送信要求	*/
#define CMD_CPRLN_FLDCOMSNDRES		/**< FLD-MTデータコマンド転送送信応答	*/
#define CMD_CPRLN_FLDCOMRCVREQ		/**< FLD-MTデータコマンド転送受信要求	*/
#define CMD_CPRLN_FLDCOMRCVRES		/**< FLD-MTデータコマンド転送受信応答	*/
#define CMD_CPRLN_FLDFILSNDREQ		/**< FLD-MTデータファイル転送送信要求	*/
#define CMD_CPRLN_FLDFILSNDRES		/**< FLD-MTデータファイル転送送信応答	*/
#define CMD_CPRLN_FLDFILRCVREQ		/**< FLD-MTデータファイル転送受信要求	*/
#define CMD_CPRLN_FLDFILRCVRES		/**< FLD-MTデータファイル転送受信応答	*/
#define CMD_CPRLN_FLMTCNCRELREQ		/**< FLD-MT接続解放要求				*/
#define CMD_CPRLN_FLMTCNCRELRES		/**< FLD-MT接続解放応答				*/
#define CMD_CPRLN_FLMTCMPRELREQ		/**< FLD-MT接続強制解放要求			*/
#define CMD_CPRLN_FLMTCMPRELRES		/**< FLD-MT接続強制解放応答			*/
#define CMD_CPRLN_SVMTCNTREQ		/**< SV-MT接続要求					*/
#define CMD_CPRLN_SVMTCNTRES		/**< SV-MT接続応答					*/
#define CMD_CPRLN_SVMTDATSNDREQ		/**< SV-MTデータ送信要求			*/
#define CMD_CPRLN_SVMTDATSNDRES		/**< SV-MTデータ送信応答			*/
#define CMD_CPRLN_SVMTDATRCVREQ		/**< SV-MTデータ受信要求			*/
#define CMD_CPRLN_SVMTDATRCVRES		/**< SV-MTデータ受信応答			*/
#define CMD_CPRLN_SVMTCNTRELREQ		/**< SV-MT接続解放要求				*/
#define CMD_CPRLN_SVMTCNTRELRES		/**< SV-MT接続解放応答				*/
#define CMD_CPRLN_SVMTFORRELREQ		/**< SV-MT接続強制解放要求			*/
#define CMD_CPRLN_SVMTFORRELRES		/**< SV-MT接続強制解放応答			*/
#define CMD_CPRLN_MKMTDATSND		/**< メーカMTデータ送信				*/
#define CMD_CPRLN_MKMTCNTREQ		/**< メーカMT接続要求				*/
#define CMD_CPRLN_MKMTCNTRES		/**< メーカMT接続応答				*/
#define CMD_CPRLN_MKMTCNTRELREQ		/**< メーカMT接続解放要求			*/
#define CMD_CPRLN_MKMTCNTRELRES		/**< メーカMT接続解放応答			*/
#define CMD_CPRLN_MKMTFORRELREQ		/**< メーカMT接続強制解放要求		*/
#define CMD_CPRLN_MKMTFORRELRES		/**< メーカMT接続強制解放応答		*/


/****************************************************************************/
/*	- No.12 -																*/
/*				タスクID定義												*/
/*																			*/
/****************************************************************************/
#if 0
/** @name task ID definition */
/* @{ */
/* ======================================================================== */
/* task ID definition 														*/
/* ======================================================================== */

#define CMD_TSKID_INI			0x00000001	/**< INI task					*/
#define CMD_TSKID_CPR			0x00000002	/**< CPRI manage task			*/
#define CMD_TSKID_LPB			0x00000003	/**< LAPB task					*/
#define CMD_TSKID_POL			0x00000004	/**< Polling task				*/
#define CMD_TSKID_TXS			0x00000005	/**< TXIOCS task				*/
#define CMD_TSKID_RXS			0x00000006	/**< RXIOCS task				*/
#define CMD_TSKID_LNS			0x00000007	/**< CPRI-MT/RE-MT LAN send task*/
#define CMD_TSKID_LNR			0x00000008	/**< CPRI-MT/RE-MT LAN receive task	*/
#define CMD_TSKID_MKM			0x00000009	/**< CPRI-MT/RE-MT manage task	*/
#define CMD_TSKID_IDL			0x0000000A	/**< IDL task					*/
#define CMD_TSKID_WDT			0x0000000B	/**< WDT task				*/
#define CMD_TSKID_DIA			0x0000000C	/**< DIA task					*/
#define CMD_TSKID_DLM			0x0000000D	/**< Download management task	*/
#define CMD_TSKID_LOG			0x0000000E	/**< Fault log task				*/
#define CMD_TSKID_RSV			0x0000000F	/**< RE supervision task		*/
#define CMD_TSKID_RCT			0x00000010	/**< RE control task			*/
#define CMD_TSKID_EXT			0x00000011	/**< External device send/receive task	*/
#define CMD_TSKID_MTM			0x00000012	/**< FLD-MT/SV-MT manage task	*/
#define CMD_TSKID_MLS			0x00000013	/**< FLD-MT/SV-MT LAN send task	*/
#define CMD_TSKID_MLR			0x00000014	/**< FLD-MT/SV-MT LAN receive task	*/
#define CMD_TSKID_TDL			0x00000015	/**< TRA file trnasfer task		*/
#define CMD_TSKID_SSV			0x00000016	/**< Sensor supervision task@800M */
#define CMD_TSKID_RAL			0x00000017	/**< Resource Alarm Log Writing task */
#define CMD_TSKID_ACT			0x00000018	/**< ACT task		*/
#define CMD_TSKID_TEL			0x00000019	/**< Telnet task		*/
/* @} */
#endif
/****************************************************************************/
/*	- No.3 -																*/
/*								タイマID定義								*/
/*																			*/
/****************************************************************************/
/** @name timer ID definition */
/* @{ */

/* ======================================================================== */
/* timer ID definition 														*/
/* ======================================================================== */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_TIMID_HCREQ_CNF_3G		E_RRH_TIMID_HCREQ_CNF_3G	/**< RSV: Health check confirm timer for 3G*/
#define CMD_TIMID_HCREQ_CNF_S3G		E_RRH_TIMID_HCREQ_CNF_S3G	/**< RSV: Health check confirm timer for S3G*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_TIMEOUTNTC_LOGTRNS_3G	0x02000010	/**< LOG：障害ログ転送間隔タイムアウト発生通知 for 3G 	*/
#define CMD_TIMEOUTNTC_LOGTRNS_S3G	0x02000011  /**< LOG：障害ログ転送間隔タイムアウト発生通知 for S3G 	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_TIMEOUTNTC_LOGTRNS_LTE	0x02000011  /**< LOG：障害ログ転送間隔タイムアウト発生通知 for LTE 	*/
#define CMD_TIMEOUTNTC_LOGGETCF		0x02000012  /**< LOG：配下REのRE障害ログ取得確認タイマタイムアウト発生通知 		*/
#define CMD_TIMEOUTNTC_LOGSNDCF		0x02000013  /**< LOG：配下REのRE障害ログ送信確認タイマタイムアウト発生通知 		*/
#define CMD_TIMEOUTNTC_LOGGETSTPCF	0x02000014  /**< LOG：配下REのRE障害ログ取得中止確認タイマタイムアウト発生通知 	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_TIMEOUTNTC_CSCDLOGGET	E_RRH_TIMID_CSCD_TROUBLE_LGRT	/**< LOG：cascadeRRE：子RE障害ログ取得応答TO 				*/
#define CMD_TIMEOUTNTC_CSCDLOGCMP	E_RRH_TIMID_CSCD_TROUBLE_LCRP   /**< LOG：cascadeRRE：子RE障害ログ受信完了通知TO			*/
#define CMD_TIMEOUTNTC_CSCDLOGFLD	E_RRH_TIMID_CSCD_TROUBLE_FILD   /**< LOG：cascadeRRE：子RE障害ログファイル分割転送TO	*/
#define CMD_TIMEOUTNTC_CSCDLOGSTP	E_RRH_TIMID_CSCD_TROUBLE_LSRT	/**< LOG：cascadeRRE：子RE障害ログ中断応答TO 				*/

#define CMD_TIMID_WARMUP0			E_RRH_TIMID_WARMUP0			/**< RSV: #0 Warm up timer */
#define CMD_TIMID_WARMUP1			E_RRH_TIMID_WARMUP1			/**< RSV: #1 Warm up timer */

#define CMD_TIMID_USEFIRMWARESND	E_RRH_TIMID_USEFIRMWARESND	/**< DLM：FIRMWARE sending in USE state confirm timer	*/
#define CMD_TIMID_SYSPRMSND_3G		E_RRH_TIMID_SYSPRMSND_3G	/**< DLM：SYSTEMPARAMETER sending confirm timer for 3G	*/
#define CMD_TIMID_SYSPRMSND_S3G		E_RRH_TIMID_SYSPRMSND_S3G	/**< DLM：SYSTEMPARAMETER sending confirm timer for 3G	*/											
#define CMD_TIMID_FIRMWARESND_3G	E_RRH_TIMID_FIRMWARESND_3G	/**< DLM：FIRMWARE sending confirm timer for 3G			*/	
#define CMD_TIMID_FIRMWARESND_S3G	E_RRH_TIMID_FIRMWARESND_S3G	/**< DLM：FIRMWARE sending confirm timer for S3G		*/	

#define CMD_TIMID_LPB_T1			E_RRH_TIMID_LPB_T1			/**< LPB：応答確認用タイマT1(link 1)					*/
#define CMD_TIMID_LPB_T2			E_RRH_TIMID_LPB_T2			/**< LPB：応答送信用タイマT2(link 1)					*/
#define CMD_TIMID_LPB_T3			E_RRH_TIMID_LPB_T3			/**< LPB：相手局ビジー監視タイマT3(link 1)				*/
#define CMD_TIMID_LPB_T4			E_RRH_TIMID_LPB_T4			/**< LPB：自局相手局アイドル状態監視タイマT4(link 1)	*/
#define CMD_TIMID_L2BSYDET			E_RRH_TIMID_L2BSYDET		/**< LPB：L2 busy detect間隔タイマ	*/
#define CMD_TIMID_L1BSYDET			E_RRH_TIMID_L1BSYDET		/**< LPB：L1 busy detect間隔タイマ	*/

#define CMD_TIMID_STBL_PA0			E_RRH_TIMID_STBL_PA0		/**< RSV: #0 PA安定化タイマ 		*/
#define CMD_TIMID_STBL_PA1			E_RRH_TIMID_STBL_PA1		/**< RSV: #1 PA安定化タイマ 		*/

#define CMD_TIMID_TILEXTDRCF_3G		E_RRH_TIMID_TILEXTDRCF_3G	/**< EXT：外部装置(TILT)データ受信確認 for 3G	 	*/
#define CMD_TIMID_TILEXTDRCF_S3G	E_RRH_TIMID_TILEXTDRCF_S3G	/**< EXT：外部装置(TILT)データ受信確認 for S3G		*/															

#define CMD_TIMID_SMCONCF_S3G		E_RRH_TIMID_SMCONCF_S3G		/**< SV-MT接続確認タイマ for S3G				*/
#define CMD_TIMID_SMRELCF_S3G		E_RRH_TIMID_SMRELCF_S3G		/**< SV-MT接続解放確認タイマ for S3G			*/
#define CMD_TIMID_SMDATRXCF_S3G		E_RRH_TIMID_SMDATRXCF_S3G	/**< SV-MTデータ受信確認タイマ for S3G			*/
															
#define CMD_TIMID_FMCONCF			E_RRH_TIMID_FMCONCF			/**< FLD-MT接続確認タイマ						*/
#define CMD_TIMID_FMRELCF			E_RRH_TIMID_FMRELCF			/**< FLD-MT接続解放確認タイマ					*/
#define CMD_TIMID_FMCOMRXCF			E_RRH_TIMID_FMCOMRXCF		/**< FLD-MTデータコマンド転送受信応答確認タイマ	*/
#define CMD_TIMID_FMFILRXCF			E_RRH_TIMID_FMFILRXCF		/**< FLD-MTデータファイル受信確認タイマ			*/
#define CMD_TIMID_SMCONCF			E_RRH_TIMID_SMCONCF			/**< SV-MT接続確認タイマ						*/
#define CMD_TIMID_SMRELCF			E_RRH_TIMID_SMRELCF			/**< SV-MT接続解放確認タイマ					*/
#define CMD_TIMID_SMDATRXCF			E_RRH_TIMID_SMDATRXCF		/**< SV-MTデータ受信確認タイマ					*/
#define CMD_TIMID_FMCONCF_S3G		E_RRH_TIMID_FMCONCF_S3G		/**< FLD-MT接続確認タイマ for S3G				*/
#define CMD_TIMID_FMRELCF_S3G		E_RRH_TIMID_FMRELCF_S3G		/**< FLD-MT接続解放確認タイマ for S3G			*/
#define CMD_TIMID_FMCOMRXCF_S3G		E_RRH_TIMID_FMCOMRXCF_S3G	/**< FLD-MTデータコマンド転送受信応答確認タイマ	for S3G */
#define CMD_TIMID_FMFILRXCF_S3G		E_RRH_TIMID_FMFILRXCF_S3G	/**< FLD-MTデータファイル受信確認タイマ	for S3G */
#define CMD_TIMID_TCPCONNWT			E_RRH_TIMID_TCPCONNWT		/**<TCP connection waiting timer 				*/
#define CMD_TIMID_FTPFILUPWT		E_RRH_TIMID_FTPFILUPWT		/**<FTP file upload waiting timer 				*/
#define CMD_TIMID_REFLDL			E_RRH_TIMID_REFLDL			/**< MKM : ダウンロード確認タイマ				*/
#define CMD_TIMID_TDLWRTSV			E_RRH_TIMID_TDLWRTSV		/**< MKM : TRAファイルライトタイマ 				*/

#define CMD_TIMID_MKM_MON			E_RRH_TIMID_MKM_MON			/**< MKM: Monitor data timer					*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*From here is FHM timer*/
/**< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for 3G */
#define CMD_TIMID_SVMT_CONN_REL_FORCE_3G	 						E_RRH_TIMID_SVMT_CONN_REL_FORCE_3G
/**< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for LTE */
#define CMD_TIMID_SVMT_CONN_REL_FORCE_LTE	 						E_RRH_TIMID_SVMT_CONN_REL_FORCE_LTE
/**< FLD-MTデータコマンド転送受信応答確認タイマ(1000msec) for 3G */
#define CMD_TIMID_FMCOMRXCF_3G			 							E_RRH_TIMID_FMCOMRXCF_3G
/**< FLD-MTデータコマンド転送受信応答確認タイマ(1000msec) for LTE */
#define CMD_TIMID_FMCOMRXCF_LTE			 							E_RRH_TIMID_FMCOMRXCF_LTE
/**< FLD-MTデータファイル転送受信確認タイマ(1000msec) for 3G */
#define CMD_TIMID_FMFILSNDCF_3G			 							E_RRH_TIMID_FMFILSNDCF_3G
/**< FLD-MTデータファイル転送受信確認タイマ(1000msec) for LTE */
#define CMD_TIMID_FMFILSNDCF_LTE		 							E_RRH_TIMID_FMFILSNDCF_LTE
/**< RE装置構成情報報告確認タイマ min timer ID  */
#define CMD_TIMID_EQPSTRMIN								 			E_RRH_TIMID_EQPSTRMIN
/**< RE装置構成情報報告確認タイマ max timer ID  */
#define CMD_TIMID_EQPSTRMAX											E_RRH_TIMID_EQPSTRMAX
/**<RE状態報告確認タイマ min timer ID  */
#define CMD_TIMID_STAREQMIN											E_RRH_TIMID_STAREQMIN
/**<RE状態報告確認タイマ max timer ID  */
#define CMD_TIMID_STAREQMAX		 									E_RRH_TIMID_STAREQMAX
/**< FLD-MT接続確認タイマ for 3G*/
#define CMD_TIMID_FLD_MT_CONN_CONF_3G	 							E_RRH_TIMID_FLD_MT_CONN_CONF_3G
/**< FLD-MT接続確認タイマ for LTE*/
#define CMD_TIMID_FLD_MT_CONN_CONF_LTE	 							E_RRH_TIMID_FLD_MT_CONN_CONF_LTE
/**< SV-MT/eNB-MT接続確認タイマ for 3G*/
#define CMD_TIMID_SVeNB_MT_CONN_CONF_3G	 							E_RRH_TIMID_SVeNB_MT_CONN_CONF_3G
/**< SV-MT/eNB-MT接続確認タイマ for LTE*/
#define CMD_TIMID_SVeNB_MT_CONN_CONF_LTE 							E_RRH_TIMID_SVeNB_MT_CONN_CONF_LTE

/**<  FLD-MT接続強制解放確認タイマ(1000msec) for 3G */
#define CMD_TIMID_FLD_MT_CONN_REL_FORCE_3G							E_RRH_TIMID_FLD_MT_CONN_REL_FORCE_3G
/**<  FLD-MT接続強制解放確認タイマ(1000msec) for LTE */
#define CMD_TIMID_FLD_MT_CONN_REL_FORCE_LTE	 						E_RRH_TIMID_FLD_MT_CONN_REL_FORCE_LTE

/**< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for 3G */
#define CMD_TIMID_SV_MT_CONN_3G										E_RRH_TIMID_SV_MT_CONN_3G
/**< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for LTE */
#define CMD_TIMID_SV_MT_CONN_LTE									E_RRH_TIMID_SV_MT_CONN_LTE

#define CMD_TIMID_TRXSET_3GMIN										E_RRH_TIMID_TRX_SET_3GMIN 		/**< TRXSET(200msec) for 3G */
#define CMD_TIMID_TRXSET_3GMAX										E_RRH_TIMID_TRX_SET_3GMAX 		/**< TRXSET(200msec) for 3G */
#define CMD_TIMID_TRXSET_LTEMIN	 									E_RRH_TIMID_TRX_SET_LTEMIN 		/**< TRXSET(200msec) for LTE */
#define CMD_TIMID_TRXSET_LTEMAX	 									E_RRH_TIMID_TRX_SET_LTEMAX 		/**< TRXSET(200msec) for LTE */

#define CMD_TIMID_TRXREL_3GMIN										E_RRH_TIMID_TRX_REL_3GMIN 		/**< TRXREL(200msec) for 3G */
#define CMD_TIMID_TRXREL_3GMAX										E_RRH_TIMID_TRX_REL_3GMAX 		/**< TRXREL(200msec) for 3G */
#define CMD_TIMID_TRXREL_LTEMIN	 									E_RRH_TIMID_TRX_REL_LTEMIN 		/**< TRXREL(200msec) for LTE */
#define CMD_TIMID_TRXREL_LTEMAX	 									E_RRH_TIMID_TRX_REL_LTEMAX 		/**< TRXREL(200msec) for LTE */

/*FLD-MTデータコマンド転送送信確認タイマ(3G)*/
#define CMD_TIMID_MT_DATA_TRANS_SND_3G								E_RRH_TIMID_MT_DATA_TRANS_SND_3G
/*FLD-MTデータコマンド転送送信確認タイマ(LTE)*/
#define CMD_TIMID_MT_DATA_TRANS_SND_LTE								E_RRH_TIMID_MT_DATA_TRANS_SND_LTE

/**< REスロット情報報告確認タイマ(200msec) for 3G */
#define CMD_TIMID_RE_SLOT_CARD_INF_3GMIN							E_RRH_TIMID_RE_SLOT_CARD_INF_3GMIN
#define CMD_TIMID_RE_SLOT_CARD_INF_3GMAX							E_RRH_TIMID_RE_SLOT_CARD_INF_3GMAX

/**< REスロット情報報告確認タイマ(200msec) for LTE */
#define CMD_TIMID_RE_SLOT_CARD_INF_LTEMIN							E_RRH_TIMID_RE_SLOT_CARD_INF_LTEMIN
#define CMD_TIMID_RE_SLOT_CARD_INF_LTEMAX							E_RRH_TIMID_RE_SLOT_CARD_INF_LTEMAX

/* SV-MT/eNB-MTデータ受信確認タイマ(10msec) for 3G */
#define CMD_TIMID_SVMTDATRCV_3G					 					E_RRH_TIMID_SVMTDATRCV_3G
/* SV-MT/eNB-MTデータ受信確認タイマ(10msec) for LTE */
#define CMD_TIMID_SVMTDATRCV_LTE		 							E_RRH_TIMID_SVMTDATRCV_LTE

/* SV-MT/eNB-MT接続解放確認タイマ(10msec) for 3G		*/
#define CMD_TIMID_SVMTCNTREL_3G		 								E_RRH_TIMID_SVMTCNTREL_3G
/* SV-MT/eNB-MT接続解放確認タイマ(10msec) for LTE		*/
#define CMD_TIMID_SVMTCNTREL_LTE		 							E_RRH_TIMID_SVMTCNTREL_LTE

/* 外部装置データ送信確認タイマ(1000ms) for 3G */
#define CMD_TIMID_EXT_DAT_SND_3G	 E_RRH_TIMID_EXT_DAT_SND_3G
/* 外部装置データ送信確認タイマ(1000ms) for LTE */
#define CMD_TIMID_EXT_DAT_SND_LTE	 E_RRH_TIMID_EXT_DAT_SND_LTE

/* 外部装置データ受信確認タイマ for 3G */
#define CMD_TIMID_EXT_DAT_RCV_3G	 E_RRH_TIMID_EXT_DAT_RCV_3G
/* 外部装置データ受信確認タイマ for LTE */
#define CMD_TIMID_EXT_DAT_RCV_LTE	 E_RRH_TIMID_EXT_DAT_RCV_LTE

/* FLD-MTデータファイル転送送信確認タイマ (3G)*/
#define CMD_TIMID_FLD_FILE_SND_3G									E_RRH_TIMID_FLD_FILE_SND_3G
/* FLD-MTデータファイル転送送信確認タイマ (LTE)*/
#define CMD_TIMID_FLD_FILE_SND_LTE									E_RRH_TIMID_FLD_FILE_SND_LTE

/*SV-MT/eNB-MTデータ送信確認タイマ for 3G */
#define CMD_TIMID_SVMTDATSND_3G		 								E_RRH_TIMID_SVMTDATSND_3G
/*SV-MT/eNB-MTデータ送信確認タイマ for LTE */
#define CMD_TIMID_SVMTDATSND_LTE		 							E_RRH_TIMID_SVMTDATSND_LTE

/* REカード情報報告応答確認待ちタイマ for 3G */
#define CMD_TIMID_CARDINFOREP_3G									E_RRH_TIMID_CARDINFOREP_3G

/*FLD-MTデータファイル転送受信応答確認タイマ for 3G */
#define CMD_TIMID_FLDMT_FILE_TRANS_3G		 						E_RRH_TIMID_FLDMT_FILE_TRANS_3G
/*FLD-MTデータファイル転送受信応答確認タイマ for LTE */
#define CMD_TIMID_FLDMT_FILE_TRANS_LTE				 				E_RRH_TIMID_FLDMT_FILE_TRANS_LTE

/* MTアドレス設定確認タイマ for 3G*/
#define CMD_TIMID_MTADDSET_3G										E_RRH_TIMID_MTADDSET_3G
/* MTアドレス設定確認タイマ for LTE*/
#define CMD_TIMID_MTADDSET_LTE										E_RRH_TIMID_MTADDSET_LTE
/* REバージョン報告確認タイマ for 3G */
#define CMD_TIMID_RE_VER_REP_3G										E_RRH_TIMID_RE_VER_REP_3G
/* REバージョン報告確認タイマ for LTE */
#define CMD_TIMID_RE_VER_REP_LTE									E_RRH_TIMID_RE_VER_REP_LTE

/* REカード情報報告応答確認待ちタイマ for 3G */



#define CMD_TIMID_FWFILE_REP_RE_LTEMIN	 							E_RRH_TIMID_FWFILE_REP_RE_LTEMIN 	/**< FWFILE_UPDATE for LTE */
#define CMD_TIMID_FWFILE_REP_RE_LTEMAX	 							E_RRH_TIMID_FWFILE_REP_RE_LTEMAX 	/**< FWFILE_UPDATE for LTE */
#define CMD_TIMID_USFWFILE_REP_RE_LTEMIN	 						E_RRH_TIMID_USFWFILE_REP_RE_LTEMIN 	/**< FWFILE_UPDATE for LTE */
#define CMD_TIMID_USFWFILE_REP_RE_LTEMAX	 						E_RRH_TIMID_USFWFILE_REP_RE_LTEMAX 	/**< FWFILE_UPDATE for LTE */
/* ファイルデータ送信間隔タイマ */
#define CMD_TIMID_FILEDATSND										E_RRH_TIMID_FILEDATSND
#define CMD_TIMID_FILEDATSND_MAX									E_RRH_TIMID_FILEDATSND_MAX
/* システムパラメータ情報報告確認タイマ */
#define CMD_TIMID_SYSPRM_INF										E_RRH_TIMID_SYSPRM_INF
#define CMD_TIMID_SYSPRM_INF_MAX									E_RRH_TIMID_SYSPRM_INF_MAX
/* システムパラメータ送信完了確認タイマ */
#define CMD_TIMID_SYSPRM_SNDFIN										E_RRH_TIMID_SYSPRM_SNDFIN
#define CMD_TIMID_SYSPRM_SNDFIN_MAX									E_RRH_TIMID_SYSPRM_SNDFIN_MAX
/* ファームウェアファイル報告確認タイマ */
#define CMD_TIMID_FIRMFILE_REP_3G									E_RRH_TIMID_FIRMFILE_REP_3G
#define CMD_TIMID_FIRMFILE_REP_3G_MAX								E_RRH_TIMID_FIRMFILE_REP_3G_MAX
#define CMD_TIMID_FIRMFILE_REP										E_RRH_TIMID_FIRMFILE_REP
#define CMD_TIMID_FIRMFILE_REP_MAX									E_RRH_TIMID_FIRMFILE_REP_MAX
/* ファームウェアファイル情報報告確認タイマ */
#define CMD_TIMID_FIRMFILE_INF										E_RRH_TIMID_FIRMFILE_INF
#define CMD_TIMID_FIRMFILE_INF_MAX									E_RRH_TIMID_FIRMFILE_INF_MAX
/* ファームウェア送信完了確認タイマ */
#define CMD_TIMID_FIRMFILE_SNDFIN									E_RRH_TIMID_FIRMFILE_SNDFIN
#define CMD_TIMID_FIRMFILE_SNDFIN_MAX								E_RRH_TIMID_FIRMFILE_SNDFIN_MAX
/* ファームウェアファイル報告確認タイマ(更新用) */
#define CMD_TIMID_UFIRMFILE_REP										E_RRH_TIMID_UFIRMFILE_REP
#define CMD_TIMID_UFIRMFILE_REP_MAX									E_RRH_TIMID_UFIRMFILE_REP_MAX
/* ファームウェア送信完了確認タイマ(更新用) */
#define CMD_TIMID_UFIRMFILE_SNDFIN									E_RRH_TIMID_UFIRMFILE_SNDFIN
#define CMD_TIMID_UFIRMFILE_SNDFIN_MAX								E_RRH_TIMID_UFIRMFILE_SNDFIN_MAX
/* ファームウェアファイル情報報告確認タイマ(更新用) */
#define CMD_TIMID_UFIRMFILE_INF										E_RRH_TIMID_UFIRMFILE_INF
#define CMD_TIMID_UFIRMFILE_INF_MAX									E_RRH_TIMID_UFIRMFILE_INF_MAX

/* RE立ち上げタイマ */
#define	CMD_TIMID_STARTUPRE											E_RRH_TIMID_STARTUPRE
/* Step7完了待ちタイマ */
#define CMD_TIMID_WAIT_COMPLETE_STEP7                               E_RRH_TIMID_WAIT_COMPLETE_STEP7
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */




/****************************************************************************/
/* Timer value defination*/
/****************************************************************************/
#define CMD_TIMVAL_WARMUP		6000		/**< Warm up timer value: 60000ms(1 minutes) */
#define CMD_TIMVAL_STBL_PA		20			/**< PA安定化時間 : 200ms */
#define CMD_TIMVAL_HWWAIT_SLP	90			/**< SLP HW制御待ち時間: 900ms */
#define CMD_TIMVAL_STBL_USECHG	21			/**< USE我ｻｯｰｲｶｨｻｯ瓶馮 : 200｡ｫ210msec */

#define CMD_TIMVAL_FANALM		100			/**< FAN ALM detect timer value */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* From here is timer val FHM timer value */

/**< REスロット情報報告確認タイマ( 200msec ) */
#define CMD_TIMVAL_RE_SLOT_CARD_INF			200

/**< MTアドレス設定確認タイマ(1000msec)*/
#define CMD_TIMVAL_MT_ADDR_SETTING			1000

/**< TRX設定確認タイマ(200msec)*/
#define CMD_TIMVAL_TRX_SET	 				200
/**< TRX解放確認タイマ(200msec)*/
#define CMD_TIMVAL_TRX_REL	 				200

/**< 外部装置データ送信確認タイマ(1000msec)*/
#define CMD_TIMVAL_EXTDATASND			 	1000

/**< FLD-MT接続強制解放確認タイマ(1000msec)*/
#define CMD_TIMVAL_FLDMT_CONN_REL_FORCE		1000
/**< FLD-MTデータコマンド転送送信確認タイマ(1000msec)*/
#define CMD_TIMVAL_MT_DATA_TRANS_SND		1000
/**< FLD-MTデータファイル転送送信確認タイマ(1000msec)*/
#define CMD_TIMVAL_FMFILSNDCF			 	1000

/**< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec)*/
#define CMD_TIMVAL_SVMT_CONN_REL_FORCE	 	1000
/**< SV-MT/eNB-MTデータ送信確認タイマ(1000msec) */
#define CMD_TIMVAL_SVMT_DATA_SND		 	1000
/**< RE障害ログ中止確認タイマ(1000msec) */
#define CMD_TIMVAL_LOGGETSTOP			 	1000

//暫定措置 2015/11/08 enoki
///* RE立ち上げタイマ(350000msc) */
//#define CMD_TIMVAL_STARTUP_RE				350000
///* Step7完了待ちタイマ(10000msec) */
//#define CMD_TIMVAL_WAIT_COMPLETE_STEP7      10000
/* RE立ち上げタイマ(340000msec) */
#define CMD_TIMVAL_STARTUP_RE				340000
/* RE立ち上げ待ち限界タイマ(360sec) */
#define CMD_TIMVAL_LIMIT_RESTARTUPE			360
/* Step7完了待ちタイマ(20000msec) */
#define CMD_TIMVAL_WAIT_COMPLETE_STEP7      20000

/* システムパラメータ情報報告確認タイマ(1000msec) */
#define CMD_TIMVAL_SYSPRM_INF				1000
/* ファームウェアファイル報告確認タイマ(25000msec) */
#define CMD_TIMVAL_FIRMFILE_REP				25000

/* ファームウェアファイル報告確認タイマ(更新用)(10000msec) */
#define CMD_TIMVAL_UFIRMFILE_REP			10000

/* ファームウェアファイル情報報告確認タイマ(1000msec) */
#define CMD_TIMVAL_FIRMFILE_INF_REP			1000

/* ファームウェアファイル情報報告確認タイマ(更新用)(10000msec) */
#define CMD_TIMVAL_UFIRMFILE_INF_REP		10000

/* システムパラメータ送信完了確認タイマ(8000msec) */
#define CMD_TIMVAL_SYSPRM_SNDFIN			8000

/* ファームウェア送信完了確認タイマ(160000 msec) */
#define CMD_TIMVAL_FIRMFILE_SNDFIN			160000

/* ファイルデータ送信間隔タイマ(150 msec) */
#define CMD_TIMVAL_FILEDATSND_INTERVAL		150

/* ファームウェア送信完了確認タイマ(更新用)(288000 msec) */
#define CMD_TIMVAL_UFIRMFILE_SNDFIN			288000

/**< RE障害ログ取得確認タイマ */
#define CMD_TIMVAL_RE_TROUBLELOG_GET_CONF	160

/**< RE障害ログ送信確認タイマ */
#define CMD_TIMVAL_RE_TROUBLELOG_SND_CONF	8550//*400 = 3420000

/**< RE障害ログ中止確認タイマ */
#define CMD_TIMVAL_RE_TROUBLELOG_STOP_CONF	120000

/** REカード情報報告確認タイマ */
#define CMD_TIMVAL_RE_CARDINFOREP_CONF		3000
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_TIMVAL_CPS_LINKUP_SV	7500	/**< CPSリンクUP監視時間: 75000ms(75 second) */
#define CMD_TIMVAL_CPS_LINKREUP_SV	1000	/**< 再接続時CPSリンクUP監視時間: 10000ms(10 second) */

/****************************************************************************/
/*	- No.5 -																*/
/*							起動要因コード定義								*/
/*																			*/
/****************************************************************************/
/** @name task IF definition */
/* @{ */

/* ======================================================================== */
/* タスク間インタフェース起動要因番号定義									*/
/* ======================================================================== */

/*	Task IF ID (0xAABB) ruler 																	*/
/*	. BB is odd number: Request or Notice message												*/
/*	. BB is even number: Response message														*/
/*	. AA is source Task ID of request or Notice message 										*/
/*	. AA is FF : common definition for CPRI signal receive and send								*/
/*	For example, INI task ID is 0x00000001, TaskIF ID for the rquest message from INI task 		*/
/*	and response message from other task is 0x01xx												*/ 

#define CMD_TSKIF_INI_BOOT		0x0000		/**< 初期化起動OS					*/

/* Request or Notice message is from INI task	*/

#define CMD_TSKIF_INIREQ		0x0101		/**< 初期化要求						*/
#define CMD_TSKIF_INIRSP		0x0102		/**< 初期化応答						*/
#define CMD_TSKIF_ALLTSKININTC	0x0103		/**< All task init complete notice */

/* Request or Notice message is from CPR task	*/

#define CMD_TSKIF_L2DEACTREQ		0x0201		/**< L2 DEACT依頼						*/
#define CMD_TSKIF_ESTREQ			0x0203		/**< データリンク設定要求				*/
#define CMD_TSKIF_L2STPREQ			0x0205		/**< L2 STOP依頼						*/
#define CMD_TSKIF_L3MODESTRNTC		0x0207		/**< L3 test 開始通知					*/
#define CMD_TSKIF_L3MODESTPNTC		0x0209		/**< L3 test 停止通知					*/
#define CMD_TSKIF_CPRIDSCNTC  		0x020B		/**< CPRIリンク断通知					*/
#define CMD_TSKIF_CPRISTANTC		0x020D		/**< CPRIリンク state通知			*/
#define CMD_TSKIF_CPRIACTNEGNTC		0x020F 		/**< CPRIリンクActive / 
															Negative変化通知 */
#define CMD_TSKIF_L3STANTC			0x0211		/**< L3リンク state通知				*/
#define CMD_TSKIF_RE_SET_REQ		0x0213		/**< 再設定要求							*/
#define CMD_TSKIF_RE_SET_RSP		0x0214		/**< 再設定応答							*/


/* Request or Notice message is from LPB task	*/
#define CMD_TSKIF_CPRIRCVNTC	0x0301		/**< CPRI信号受信通知					*/
#define CMD_TSKIF_CPRISNDNTC	0x0303 		/**< CPRI信号送信通知					*/
#define CMD_TSKIF_L1DATTXREQ	0x0305		/**< L1データ送信要求					*/
#define CMD_TSKIF_ESTCNF		0x0307		/**< データリンク設定確認				*/
#define CMD_TSKIF_ESTIND		0x0309		/**< データリンク設定表示				*/
#define CMD_TSKIF_ESTINGIND		0x030B		/**< データリンク設定中表示				*/
#define CMD_TSKIF_RELIND		0x030C		/**< データリンク解放表示				*/
#define CMD_TSKIF_RELREQ		0x030d		/* データリンク解放要求					*/
#define CMD_TSKIF_RELCNF		0x030e		/* データリンク解放確認					*/
#define CMD_TSKIF_L2STANTC		0x030f		/* LAPB Link State変化通知				*/
#define CMD_TSKIF_L2STANTC_CSCD	0x0310		/* LAPB Link(Cascade) State変化通知		*/

/* Request or Notice message is from POL(RXS/MKM) task	*/
#define CMD_TSKIF_L1DATRCVNTC	0x0401		/**< L1データ受信通知					*/
#define CMD_TSKIF_L1DATCNF		0x0402		/**< Layer 1 Data CNF					*/

/* Request or Notice message is from TXS task	*/

#define CMD_TSKIF_TXABNMLNTC  		0x0501		/**< TX送信異常通知				*/
#define CMD_TSKIF_L1MONDATSNDNTC	0x0503	/**< L1データ送信要求(Monitor)	*/

/* Request or Notice message is from RXS task	*/

#define CMD_TSKIF_L1MONDATRCVNTC	0x0601	/**< L1データ受信通知(Monitor)	*/

/* Request or Notice message is from LNR(MLR) task	*/
#define CMD_TSKIF_TCPCONNNTC		0x0801		/**< TCP接続通知					*/
#define CMD_TSKIF_TCPRELNTC			0x0803		/**< TCP解放通知					*/
#define CMD_TSKIF_TCPDATNTC			0x0805		/**< TCP データ受信通知				*/
#define CMD_TSKIF_TCPCONNIND		0x0807
/* Request or Notice message is from MKM task	*/

#define CMD_TSKIF_MKMTSTRREQ		0x0901		/**< メーカMT運用開始通知			*/
#define CMD_TSKIF_MKMTSTPREQ		0x0903		/**< メーカMT運用停止通知			*/
#define CMD_TSKIF_CPRISIMSTRREQ		0x0905		/**< CPRI Simulator開始要求			*/
#define CMD_TSKIF_CPRISIMSTRRSP		0x0906		/**< CPRI Simulator開始応答			*/
#define CMD_TSKIF_CPRISIMSTPREQ		0x0907		/**< CPRI Simulator停止要求			*/
#define CMD_TSKIF_CPRISIMSTPRSP		0x0908		/**< CPRI Simulator停止応答			*/
#define CMD_TSKIF_CPRIMONSTRREQ		0x0909		/**< CPRI Monitor開始通知			*/
#define CMD_TSKIF_CPRIMONSTPREQ		0x090B		/**< CPRI Monitor停止通知			*/
#define CMD_TSKIF_TCPSNDREQ			0x090D		/**< TCP データ送信通知				*/
#define CMD_TSKIF_TCPSNDRSP			0x090E		/**< TCPデータ送信応答				*/
#define CMD_TSKIF_TRAFLWRTREQ		0x090F		/**< TRAファイルライト要求			*/
#define CMD_TSKIF_TRAFLWRTRSP		0x0910		/**< TRAファイルライト応答 			*/
#define CMD_TSKIF_LCKBLKCNTREQ		0x0911		/**< Local Block Control Request		*/
#define CMD_TSKIF_LCKBLKCNTRES		0x0912		/**< Local Block Control Response		*/
#if 0
#define CMD_TSKIF_POW_ADJ_REQ		0x0911		/**< 送信電力調整要求		 		*/
#define CMD_TSKIF_POW_SAV_REQ		0x0913		/**< 送信電力保存要求		 		*/
#define CMD_TSKIF_POW_READ_REQ		0x0916		/**< LRE送信電力読出要求	 		*/
#endif
/* Request or Notice message is from DLM task	*/

/* cascadeRRE対応 子RE(PL)->L3_DIag */
/*#define CMD_TSKIF_CSCDDIARCV		0x0C09*/		/**< 子REカード診断応答				*/

#define CMD_TSKIF_3GRF_TRXSET_NTC	0x0D01		/**< 3G RF TRX設定通知			*/
#define CMD_TSKIF_FWDLMREQ 			0x0D02		/**< Writing/Erasing request 	*/
#define CMD_TSKIF_FWDLMRSP 			0x0D03		/**< Writing/Erasing response	*/
#define CMD_TSKIF_SYSPARA_CHG_NTC	0x0D05		/**< System Paremeter Chg NTC	*/
#define CMD_TSKIF_TRXRLS_NTC		0x0F0F		/**< TRX解放通知				*/

/* cascadeRRE対応 親<->子 */
/*#define CMD_TSKIF_CDTLOGGETREQ		0x0E06*/		/**< 子RE障害ログ取得応答			*/
/*#define CMD_TSKIF_CDTLOGRCVTRAP		0x0E07*/		/**< 子RE障害ログ受信通知			*/
/*#define CMD_TSKIF_CDTLOGCMPTRAP		0x0E08*/		/**< 子RE障害ログ受信完了通知		*/
#define CMD_TSKIF_CDTLOGSTOPRES		0x0E09		/**< 子RE障害ログ中断応答			*/
#define CMD_TSKIF_CSCDLOGGETTO		0x0E0A		/**< 子RE障害ログ取得応答TO発生		*/
#define CMD_TSKIF_CSCDLOGCMPTO		0x0E0B		/**< 子RE障害ログ受信完了通知TO発生	*/
#define CMD_TSKIF_CSCDLOGSTPTO		0x0E0C		/**< 子RE障害ログ中断応答TO発生		*/
#define CMD_TSKIF_CSCDLOGDILDV		0x0E0D		/**< 子RE障害ログファイル分割転送TO	*/

#if 0
/* Request or Notice message is from RSV task	*/
#define CMD_TSKIF_L3EST_NTC			0x0F01		/**< L3ヘルスチェック確立通知	*/
#define CMD_TSKIF_L3DSC_NTC			0x0F03		/**< L3ヘルスチェック断通知		*/
#define CMD_TSKIF_L2DSCREQ_NTC		0x0F05		/**< L2リンク断依頼通知			*/
#define CMD_TSKIF_LNKSET_NTC		0x0F07		/**< RE共通部リンク再確立通知	*/

#define CMD_TSKIF_EXTDEVCHG_NTC		0x0F11		/* 外部装置状変通知				*/
#define CMD_TSKIF_CPR_STA_NTC		0x0F13		/**< CPR起動通知				*/
#define CMD_TSKIF_TCPSRVINITREQ		0x0F15		/**< TCP Server 初期化要求		*/
#define CMD_TSKIF_TCPSRVINITRSP		0x0F16		/**< TCP Server初期化応答		*/
#define CMD_TSKIF_SYSPARAWAIT_NTC	0x0F17		/**< SYSPARA wait start notice	*/
#define CMD_TSKIF_PA_ONOFF_REQ		0x0F19		/**< PA ON/OFF Req	 			*/
#define CMD_TSKIF_APDWAVE_ONOFF_REQ	0x0F1A		/**< APD Wave ON/OFF Req	 	*/
#endif

#define CMD_TSKIF_TCPSRVINITREQ		0x0F15		/**< TCP Server 初期化要求		*/
#define CMD_TSKIF_TCPSRVINITRSP		0x0F16		/**< TCP Server初期化応答		*/
#define CMD_TSKIF_CPR_STA_NTC		0x0F13		/**< CPR起動通知				*/
#define CMD_TSKIF_TRXRLS_NTC		0x0F0F		/**< TRX解放通知				*/
#define CMD_TSKIF_CARDSTANTC		0x0F1C		
#define CMD_TSKIF_CAS_RETIMESETREQ	0x0F1D		/**< 時刻再設定要求				*/
/* Request or Notice message is from RCT task	*/

#define CMD_TSKIF_BTISET_NTC			0x1001	/**< BTI setting notice 		*/
#define CMD_TSKIF_SLPCTRL_NTC			0x1003	/**< Sleep control notice	*/
#define CMD_TSKIF_CAR_ON_REQ			0x1005	/**< Carrier ON Req	 			*/
#define CMD_TSKIF_CAR_OFF_REQ			0x1007	/**< Carrier OFF Req	 		*/
#define CMD_TSKIF_NOMOD_CAR_ONOFF_REQ	0x1009	/**< 無変調CarrierON/OFF Req	*/

/* Request or Notice message is from MTM task	*/

#define CMD_TSKIF_TCPSRVSTRREQ	0x1201		/* TCP Server start request		*/
#define CMD_TSKIF_TCPSRVSTRRSP	0x1202		/* TCP server start response 	*/
#define CMD_TSKIF_TCPSRVRSTIND	0x1203		/* TCP server start response 	*/

/* Request or Notice message is from MLR/LNR task	*/

#define CMD_TSKIF_TCPSRVSTPNTC	0x1401		/**< TCP server停止通知				*/

/* Request or Notice message is from HDL task	*/
#define CMD_TSKIF_TIMOUTNTC			0x1601		/**< タイムアウト発生通知			*/
#define CMD_TSKIF_L2BSY				0x1603		/**< 自受信ビジー					*/
#define CMD_TSKIF_L2BSYCLR			0x1605		/**< 自受信ビジーの解除				*/
#define CMD_TSKIF_POLINTRNTC		0x1607		/**< ポーリング周期割込通知			*/
#define CMD_TSKIF_CPRISTACHGNTC		0x1609		/**< CPRI State変化通知				*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_TSKIF_TIMOUTRSPNTC			0x160B		/**< タイムアウト発生通知 RSP			*/
#define CMD_TSKIF_TIMOUTSNDNTC			0x160D		/**< タイムアウト発生通知 SND			*/
#define CMD_TSKIF_CPRIDSCSTNTC                  0x160F      /**< タイムアウト発生通知 CPRIDSCST */

#define CMD_TSKIF_RE_FWDNTC             0x4001      /**< RE Forwarding Notice */
#define CMD_TSKIF_REC_FWDNTC            0x4003      /**< REC Forwarding Notice */
#define CMD_TSKIF_RE_STARTUP_REQ        0x4005      /**< RE Startup Request */
#define CMD_TSKIF_RE_STARTUP_RSP        0x4006      /**< RE Startup Response */
#define CMD_TSKIF_RE_DISCONNECT_REQ     0x4007      /**< RE Disconnection Request */
#define CMD_TSKIF_RE_DISCONNECT_RSP     0x4008      /**< RE Disconnection Response */
#define CMD_TSKIF_RE_SLFRST_PRVTOFF_REQ 0x4009      /**< RE自律リセット抑止解除要求 */

#define CMD_TSKIF_REC_CPRI_PRE_REQ		0x4101		/**< REC CPRI Link Preparation Request	*/
#define CMD_TSKIF_REC_CPRI_PRE_RSP   	0x4102      /**< REC CPRI Link Preparation Response */
#define CMD_TSKIF_REC_CPRI_START_NTC	0x4103		/**< REC CPRI Link Start Notice */
#define CMD_TSKIF_RE_CPRI_START_NTC		0x4105		/**< RE CPRI Link Start Notice */
#define CMD_TSKIF_CPRI_CONNECT_NTC      0x4107      /**< CPRI Connection Notice */
#define CMD_TSKIF_CPRI_DISCONNECT_NTC   0x4109      /**< CPRI Disconnection Notice  */
#define CMD_TSKIF_RE_CPRI_STOP_NTC      0x410B      /**< RE CPRI Link Stop Notice */

#define CMD_TSKIF_REFIRM_DL_REQ         0x4201      /**< RE Firm Download Request */
#define CMD_TSKIF_REFIRM_DL_RSP         0x4202      /**< RE Firm Download Response */
#define CMD_TSKIF_REFIRM_DL_START_NTC   0x4203      /**< RE Firm Download Start Notice */
#define CMD_TSKIF_REFIRM_DL_START_RSP   0x4204      /**< RE Firm Download Start Response */
#define CMD_TSKIF_REFIRM_DL_COMP_NTC    0x4205      /**< RE Firm Download Complete Notice */
#define CMD_TSKIF_REFIRM_DL_STOP_REQ    0x4206      /**< RE Firm Download Stop Request */
#define CMD_TSKIF_REFIRM_DL_STOP_IND    0x4207      /**< RE Firm Download Stop Indication */
#define CMD_TSKIF_RESYSPARA_UPDATE_REQ  0x4211      /**< RE System Parameter Update Request */
#define CMD_TSKIF_RESYSPARA_UPDATE_RSP  0x4212      /**< RE System Parameter Update Response */
#define CMD_TSKIF_USEREFILE_UPDATE_START_RSP    0x4705      /**< 運用中REファイル更新開始応答 */
#define CMD_TSKIF_REFILE_UPDATE_COMP_NTC        0x4214	    /**< RE File Update Complete Notice */
#define CMD_TSKIF_REFILE_UPDATE_START_NTC       0x4215 	    /**< RE File Update Start Notice */
#define CMD_TSKIF_FILEDATSND_TRANS_IND          0x4216 	    /**< ファイルデータ送信転送指示*/
#define CMD_TSKIF_RESYSPARA_INIT_IND            0x4217      /**< システムパラメータ初期化指示 */

#define CMD_TSKIF_RE_FORCESTOP_IND              0x4311      /**< RE Force Stop Indication(l3_rct->re_rec) */
#define CMD_TSKIF_RE_FORCERESET_IND             0x4312      /**< RE Force Reset Indication(l3_rct->re_rec) */
#define CMD_TSKIF_RE_FORCESTOP_NTC				0x4313		/**< RE Force Stop Notice(re_rec->l3_rct) */

#define CMD_TSKIF_OBSLOG_CRE_REQ        0x4501      /**< FHM ObsLog Create Request */
#define CMD_TSKIF_OBSLOG_CRE_FIN_NTC    0x4502      /**< FHM ObsLog Create Finish Notice */
#define CMD_TSKIF_RE_OBSLOG_CRE_REQ     0x4503      /**< RE Obslog Create Request */
#define CMD_TSKIF_RE_OBSLOG_CRE_FIN_NTC 0x4504      /**< RE Obslog Create Finish Notice */

#define CMD_TSKIF_RETIMSETPRCIND        0x4601      /**< RE time set execute indication */
#define CMD_TSKIF_REPRMPRCIND           0x4602      /**< RE system parameter update execute indication */
#define CMD_TSKIF_REFILPRCIND           0x4603      /**< RE file update execute indication */
#define CMD_TSKIF_TOFVALPRCIND          0x4604      /**< Toffset report execute indication */
#define CMD_TSKIF_REEQPINFPRCIND        0x4605      /**< RE equipment composition information report execute indication */
#define CMD_TSKIF_RESLTINFPRCIND        0x4606      /**< RE slot information report execute indication */
#define CMD_TSKIF_RESTSPRCIND           0x4607      /**< RE status report execute indication */
#define CMD_TSKIF_REPRTSTSPRCIND        0x4608      /**< RE port status report execute indication */
#define CMD_TSKIF_RECRDSSCPRCIND        0x4609      /**< RE card status report execute indication (sector) */
#define CMD_TSKIF_RECRDSCMPRCIND        0x460A      /**< RE card status report execute indication (common) */
#define CMD_TSKIF_RECRRSTSPRCIND        0x460B      /**< RE carrier status  report execute indication */
#define CMD_TSKIF_MTSETFPRCIND          0x460C      /**< MT address set execute indication (FLD-MT/eNB-MT) */
#define CMD_TSKIF_RESCMPPRCIND          0x460D      /**< RE startup complete execute indication */
#define CMD_TSKIF_RERSTPRCIND           0x460E      /**< RE reset execute indication */
#define CMD_TSKIF_REFRCRSTSETPRCIND     0x460F      /**< RE force reset execute indication */
#define CMD_TSKIF_REFRCRSTPRCIND        0x4610      /**< RE force reset procedure execute indication */
#define CMD_TSKIF_FRCSTPPRCIND          0x4611      /**< RE force stop execute indication */
#define CMD_TSKIF_CPRSTACHGNTC          0x4621      /**< CPRI state change Notice */
#define CMD_TSKIF_CPRDSCNTC             0x4622      /**< CPRI断通知 */
#define CMD_TSKIF_2NDRST_REQ            0x4623      /**< 2nd reset request */
#define CMD_TSKIF_CPRIRESTARTIND        0x4624      /**< CPRI restart indication */

#define CMD_TSKIF_GETFIRMINF_REQ        0x5001      /**< Get Firmware Information Request */
#define CMD_TSKIF_GETFIRMINF_RSP        0x5002      /**< Get Firmware Information Response */
#define CMD_TSKIF_LOADFIRM_REQ          0x5003      /**< Load Firmware Data Request */
#define CMD_TSKIF_LOADFIRM_RSP          0x5004      /**< Load Firmware Data Response */
#define CMD_TSKIF_CANCEL_LOADFIRM_REQ   0x5005      /**< Cancel Load Firmware Data Request */
#define CMD_TSKIF_CANCEL_LOADFIRM_RSP   0x5006      /**< Cancel Load Firmware Data Response */
#define CMD_TSKIF_SAVEFIRM_REQ          0x5007      /**< Save Firmware File Request */
#define CMD_TSKIF_SAVEFIRM_RSP          0x5008      /**< Save Firmware File Response */
#define CMD_TSKIF_SAVE_REFILE_REQ       0x5009      /**< Save RE File Request */
#define CMD_TSKIF_SAVE_REFILE_RSP       0x500A      /**< Save RE File Response */
#define CMD_TSKIF_ERASEFIRM_REQ         0x500B      /**< Erase Firmware File Request */
#define CMD_TSKIF_ERASEFIRM_RSP         0x500C      /**< Erase Firmware File Response */

#define CMD_TSKIF_RECMPSTPNTC                   0x85020005  /**< 強制停止通知 */
#define CMD_TSKIF_CPRIDBGSTARTREQ               0x87030000  /**< CPRIデバッグ機能開始要求 */
#define CMD_TSKIF_CPRIDBGSTARTRES               0x87030001  /**< CPRIデバッグ機能開始応答 */
#define CMD_TSKIF_CPRIDBGSTOPREQ                0x87040000  /**< CPRIデバッグ機能停止要求 */
#define CMD_TSKIF_CPRIDBGSTOPRES                0x87040001  /**< CPRIデバッグ機能停止応答 */
#define CMD_TSKIF_CPRIMONISTARTIND              0x87050004  /**< CPRI運用状態監視開始指示 */
#define CMD_TSKIF_CPRIMONISTOPIND               0x87060004  /**< CPRI運用状態監視停止指示 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_TSKIF_TIMOUTNTC_CSCD		0x1631	/**< タイムアウト発生通知(Cascade)	*/
#define CMD_TSKIF_CPSLINKUPSVTIMEOUT	0x1632	/**< CPSリンクUP監視タイムアウト通知*/
#define CMD_TSKIF_CPRISTACHGNTC_CSCD 	0x1633	/**< CPRI(master) State変化通知		*/
#define CMD_TSKIF_CPRIRESETTINGNTC		0x1634	/**< 再接続時パラメータ再設定通知	*/
#define CMD_TSKIF_CPSLINKREUPSVTIMEOUT	0x1635	/**< 再接続時CPSリンクUP監視タイムアウト通知*/

#if 0
#define CMD_TSKIF_BLK_IRQNTC		0x160B		/**< ローカル閉塞閉塞解除割込通知	*/
#define CMD_TSKIF_RST_IRQNTC		0x160D		/**< ローカルリセット割込通知		*/
#define CMD_TSKIF_ALM_IRQNTC		0x160F		/**< ローカルALM割込通知			*/
#define CMD_TSKIF_TRA_IRQNTC		0x1611		/**< MDIFカード状態変化割込通知		*/
#define CMD_TSKIF_PORT_IRQNTC		0x1613		/**< PORT状態変化割込通知			*/
#define CMD_TSKIF_EXTRXNTC			0x1615		/**< 外部装置データ受信通知			*/
#define CMD_TSKIF_EXTTXENDNTC		0x1617		/**< 外部装置データ送信確認通知		*/
#define CMD_TSKIF_EXTIF_IRQNTC		0x1619		/**< 外部IF状態変化割込通知			*/
#define CMD_TSKIF_3GRF_IRQNTC		0x161B		/**< 3G RF status change interruption notice */
#define CMD_TSKIF_BREALM_IRQNTC 	0x161D		/**< BRE ALARM interruption notice	*/
#define CMD_TSKIF_SHF_IRQNTC		0x161F		/**< シェルフ状態変化割込通知		*/
#define CMD_TSKIF_PASW_IRQNTC		0x1621		/**< PA-SW change Interrupt	*/
#define CMD_TSKIF_POW_RPT_NOTI		0x1623		/**< 報告周期割り込み通知		*/
#define CMD_TSKIF_HIGH_SPEED_NOTI	0x1625		/**< 高速モード要求割り込み通知	*/
#define CMD_TSKIF_TILT_IRQNTC		0x1626		/**< 他系TILT接続状態変化割込通知	*/
#define CMD_TSKIF_PROTOUT_IRQNTC	0x1627		/**< 他系PORT出力情報変化割込通知	*/
#define CMD_TSKIF_PROTOUT_ININTC	0x1628		/**< 他系PORT出力初期化割込通知		*/

/* Request or Notice message is from SSV task	*/

#define CMD_TSKIF_SSVSTA_IND	0x1701		/**< センサ監視開始指示 @800M */
#define CMD_TSKIF_SSVSTP_IND	0x1703		/**< センサ監視停止指示 @800M */

/* Request or Notice message is from RAL task	*/

#define CMD_TSKIF_RAL_WRTREQ	0x1801		/**< リソースALM ログ取得要求 */
#define	CMD_TSKIF_RAL_WRTREQ_HC	0x1802		/**< リソース ログ取得要求 (L1リンク確立 + L3断（ヘルスチェック異常）)	*/
#endif

/* @} */



/****************************************************************************/
/*	- No.6 -																*/
/*					TRAカード内共通関数のパラメータ定義					*/
/*																			*/
/****************************************************************************/

/** @name NULL definition */
/* @{ */

#ifndef NULL
#define NULL					0			/**< NULL							*/
#endif

/* @} */

/** @name ALLF initialization for CHAR SHORT INT */
/* @{ */

#define CMD_ALLF_CHAR			0xFF		/**< ALLF initialization (CHAR)	*/
#define CMD_ALLF_SHORT			0xFFFF		/**< ALLF initialization (SHORT) */
#define CMD_ALLF_INT			0xFFFFFFFF	/**<  ALLF initialization (INT) */



/** @name flag definition */
/* @{ */

#define CMD_FLG_ON			1			/**< flag ON			*/
#define CMD_FLG_OFF			0			/**< flag OFF			*/

/* @} */

/** @name bit definition */
/* @{ */

/* Bit */
#define CMD_BIT1				1
#define CMD_BIT2				2
#define CMD_BIT3				3
#define CMD_BIT4				4
#define CMD_BIT5				5
#define CMD_BIT6				6
#define CMD_BIT7				7
#define CMD_BIT8				8
#define CMD_BIT9				9
#define CMD_BIT10				10
#define CMD_BIT11				11
#define CMD_BIT12				12
#define CMD_BIT13				13
#define CMD_BIT14				14
#define CMD_BIT15				15
#define CMD_BIT16				16

/* @} */

/** @name byte size for INT */
/* @{ */

#define CMD_BYTESIZE_INT		4	/**< byte size for INT			*/

/* @} */

/** @name number definition */
/* @{ */

/****************************************************************************/
/* common item definition ( number definition )								*/
/****************************************************************************/
#define CMD_NUM_MINUS1			-1			/**< -1							*/
#define CMD_NUM0				0			/**< 0							*/
#define CMD_NUM1				1			/**< 1							*/
#define CMD_NUM2				2			/**< 2							*/
#define CMD_NUM3				3			/**< 3							*/
#define CMD_NUM4				4			/**< 4							*/
#define CMD_NUM5				5			/**< 5							*/
#define CMD_NUM6				6			/**< 6							*/
#define CMD_NUM7				7			/**< 7							*/
#define CMD_NUM8				8			/**< 8							*/
#define CMD_NUM9				9			/**< 9							*/
#define CMD_NUM10				10			/**< 10							*/
#define CMD_NUM11				11			/**< 11							*/
#define CMD_NUM12				12			/**< 12							*/
#define CMD_NUM13				13			/**< 13							*/
#define CMD_NUM14				14			/**< 14							*/
#define CMD_NUM15				15			/**< 15							*/
#define CMD_NUM16				16			/**< 16							*/
#define CMD_NUM17				17			/**< 17							*/
#define CMD_NUM18				18			/**< 18							*/
#define CMD_NUM19				19			/**< 19							*/
#define CMD_NUM20				20			/**< 20							*/
#define CMD_NUM21				21			/**< 21							*/
#define CMD_NUM22				22			/**< 22							*/
#define CMD_NUM23				23			/**< 23							*/
#define CMD_NUM24				24			/**< 24							*/
#define CMD_NUM25				25			/**< 25							*/
#define CMD_NUM26				26			/**< 26							*/
#define CMD_NUM27				27			/**< 27							*/
#define CMD_NUM28				28			/**< 28							*/
#define CMD_NUM29				29			/**< 29							*/
#define CMD_NUM30				30			/**< 30							*/
#define CMD_NUM31				31			/**< 31							*/
#define CMD_NUM32				32			/**< 32							*/
#define CMD_NUM33				33			/**< 33							*/
#define CMD_NUM36				36			/**< 36							*/
#define CMD_NUM48				48			/**< 48							*/
#define CMD_NUM50				50			/**< 50							*/
#define CMD_NUM64				64			/**< 64							*/
#define CMD_NUM60				60			/**< 60		@800M				*/
#define CMD_NUM100				100			/**< 100						*/
#define CMD_NUM128				128			/**< 128						*/
#define CMD_NUM200				200			/**< 200						*/
#define CMD_NUM255				255			/**< 255						*/
#define CMD_NUM256				256			/**< 256						*/
#define CMD_NUM300				300			/**< 300						*/
#define CMD_NUM512				512			/**< 512						*/
#define CMD_NUM1000				1000		/**< 1000						*/
#define CMD_NUM1024				1024		/**< 1024						*/
#define CMD_NUM2048				2048		/**< 2048						*/
#define CMD_NUM4096				4096		/**< 4096						*/
#define CMD_NUM12500			12500		/**< 12500						*/
#define CMD_NUM_10MS			100000		/**< 100000						*/

#define CMD_NUM_0x104			0x104		/**< 0x104						*/
#define CMD_NUM_0x108			0x108		/**< 0x108						*/
#define CMD_NUM_0x119			0x119		/**< 0x119						*/
#define CMD_NUM_0x11C			0x11C		/**< 0x11C						*/
#define CMD_NUM_0x138			0x138		/**< 0x138						*/
#define CMD_NUM_0x13C			0x13C		/**< 0x13C						*/
#define CMD_NUM_0x170			0x170		/**< 0x170						*/
#define CMD_NUM_0x1FF			0x1FF		/**< 0x1FF						*/
#define CMD_NUM_0x1A0			0x1A0		/**< 0x1A0						*/
#define CMD_NUM_0x200			0x200		/**< 0x200						*/
#define CMD_NUM_0x2000			0x2000		/**< 0x2000						*/
#define CMD_NUM_0x10000			0x10000		/**< 0x10000					*/
#define CMD_NUM_0x80			0x80		/**< 0x80						*/
#define CMD_NUM_0x800			0x800		/**< 0x800						*/
#define CMD_NUM_0x8000			0x8000		/**< 0x8000						*/
#define	CMD_NUM_0xFF			0xFF		/**< 0xFF						*/
/* @} */

/** @name basic supervision item definition */
/* @{ */


/****************************************************************************/
/* basic supervision item definition										*/
/****************************************************************************/
#define CMD_PSTBY			0x0800		/**< PSTBY					*/
#define CMD_USE				0x0400		/**< USE					*/
#define CMD_ALM				0x0200		/**< ALM					*/
#define CMD_ERR				0x0100		/**< ERR					*/
#define CMD_INS				0x0080		/**< INS					*/
#define CMD_NOINS			0x0040		/**< NOINS					*/
#define CMD_ACT				0x0020		/**< ACT					*/
#define CMD_STBY			0x0010		/**< STBY					*/
#define CMD_PBLK			0x0008		/**< PBLK					*/
#define CMD_BLK				0x0004		/**< BLK					*/
#define CMD_CHK				0x0002		/**< CHECK					*/

/* @} */

/** @name extendable supervision item definition */
/* @{ */

/****************************************************************************/
/* Extendable supervision item												*/
/****************************************************************************/
#define CMD_AMPPWROFF			0x8000		/**< Power off					*/
#define CMD_WARMUP				0x0002		/**< WarmUp						*/

/* @} */


/** @name buffer category definition */
/* @{ */

/****************************************************************************/
/* buffer category															*/
/****************************************************************************/

#define CMD_BUFCA_TSKIF		E_BPF_RU_IPCM_BUFF_KIND_TASK		/**< TASK IF commnunication use		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_BUFCA_LAPSND	E_BPF_RU_IPCM_BUFF_KIND_LAPBS		/**< LAPB communication - sending	*/
#define CMD_BUFCA_LAPRCV	E_BPF_RU_IPCM_BUFF_KIND_LAPBR		/**< LAPB communication - receiving	*/
#define CMD_BUFCA_MON		E_BPF_RU_IPCM_BUFF_KIND_MONITOR		/**< Monitor use					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/** @name common function return code definition */
/* @{ */

/* ======================================================================== */
/* return code definition for common function 								*/
/* ======================================================================== */
#define CMD_RES_OK				0			/**< Normal end					*/
#define CMD_RES_NG				0xF0000001	/**< Abnormal end				*/
#define CMD_RES_SIZEERR			0xF0000002	/**< size specify error			*/
#define CMD_RES_EMPTY			0xF0000003	/**< resource empty				*/
#define CMD_RES_KINDERR			0xF0000004	/**< kind specify error			*/
#define CMD_RES_ADRERR			0xF0000005	/**< address specify error		*/
#define CMD_RES_NOUSE			0xF0000006	/**< select nouse resource		*/
#define CMD_RES_OVERLAP			0xF0000007	/**< double specify				*/
#define CMD_RES_PARAMERR		0xF0000008	/**< parameter specify error	*/
#define CMD_RES_ALREADYSTOP		0xF0000009	/**< double stop specify		*/
#define CMD_RES_FAILED			0xF000000A	/**< processing failure			*/
#define CMD_RES_TIMEOUT			0xF000000B	/**< processing timeout			*/
#define CMD_RES_UNSUPPORTED		0xF000000C	/**< unsupported				*/
#define CMD_RES_PARITYERR		0xF000000D	/**< parity error				*/
#define CMD_RES_RWCHKERR		0xF000000E	/**< R/W check NG				*/
#define CMD_RES_CATEGORY		0xF000000F	/**< category specify error		*/
#define CMD_RES_DOUBLEFREE		0xF0000010	/**< buffer double free specify	*/
#define CMD_RES_STOP			0xF0000011	/**< Process is stopped	*/

/* @} */

/** @name L3 signal transfer procesing return code definition */
/* @{ */

/****************************************************************************/
/* L3 transfer processing return code										*/
/****************************************************************************/
/* return code */
#define CMD_L3_OK				0		/**< OK								*/
#define CMD_L3_CPRICUT			1		/**< CPRI link down					*/
#define CMD_L3_CPRINOT			2		/*8, CPRI link not exist			*/
#define CMD_L3_API_NG			3		/**< api return NG								*/

/* @} */

/** @name CPRI link state number define */
/* @{ */

/* CPRI link state number define											*/
#define CRD_CPRILNKSTA_NO0		0			/**< CPRI stateE donw */
											/**< HDLC idle */
											/**< none-stateF */
											/**< TX abnormal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO1		1			/**< CPRI stateE */
											/**< HDLC idle */
											/**< none-stateF */
											/**< TX abnormal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO2		2			/**< CPRI stateE */
											/**< HDLC active */
											/**< none-stateF */
											/**< TX normal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO3		3			/**< CPRI stateE */
											/**< HDLC active */
											/**< none-stateF */
											/**< TX abnormal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO4		4			/**< CPRI stateE */
											/**< HDLC active */
											/**< stateF */
											/**< TX normal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO5		5			/**< CPRI stateE */
											/**< HDLC active */
											/**< stateF */
											/**< TX normal */
											/**< L3 OK */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CRD_CPRILNKSTA_NO6		6
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/** @name assert log output level definition */
/* @{ */

/* ======================================================================== */
/* Assert log output level definition										*/
/* ======================================================================== */
#define CMD_ASL_NOLEVEL			D_RRH_LOG_AST_LV_ENTER		/**< no level							*/
#define CMD_ASL_DBGLOW			D_RRH_LOG_AST_LV_INFO		/**< debug lower level (usual use)		*/
#define CMD_ASL_DBGHIGH			D_RRH_LOG_AST_LV_ERROR		/**< debug high level (analyze use)		*/
#define CMD_ASL_USELOW			D_RRH_LOG_AST_LV_WARNING	/**< use state lower level (fail-safe)	*/
#define CMD_ASL_USEHIGH			D_RRH_LOG_AST_LV_CRITICAL	/**< use state high level (RAS)			*/
#define CMD_ASL_RETURN			D_RRH_LOG_AST_LV_RETURN		/**< no level							*/

/* @} */

/** @name system call parameter definition */
/* @{ */

#define CMD_SYSCAL_EEMOD_MSK	0x00000000	/**< EEMOD interrupt mask 	*/
#define CMD_SYSCAL_EEMOD_UNMSK	0x00000001	/**< EEMOD interrupt unmask		*/

/* @} */

/** @name system call return code definition */
/* @{ */

/* ======================================================================== */
/* system call return code definition ( timer related)						*/
/* ======================================================================== */
#define CMD_SYSRES_OK				0			/**< normal						*/
#define CMD_SYSRES_TIMER_EXIST		0x000A		/**< timer exist 				*/
#define CMD_SYSRES_TIMER_NONE		0x000B		/**< timer not exist			*/

/* @} */


/* ======================================================================== */
/* Max larer 2 buffer kind													*/
/* ======================================================================== */
#define CMD_MAX_L2_BFKIND		3		

/* ======================================================================== */
/* 起動事象(QRB種別)定数定義												*/
/* ======================================================================== */
#define CMD_QRB_NORMAL			0x00000000	/**< 一般指定					*/
#define CMD_QRB_EMERGENCY		0x00000001	/**< 緊急指定					*/

/* ======================================================================== */
/* セマフォID定数定義														*/
/* ======================================================================== */
#define CMD_SEMID_ENTERLOG			0x00000001	/**< タスク起動ログ書込用	*/
#define CMD_SEMID_BUFFLOG			0x00000002	
										/**< バッファ操作履歴／未返却ログ書込用	*/
#define CMD_SEMID_TIMERLOG			0x00000003	
										/**< タイマ操作履歴／管理ログ書込用		*/
#define CMD_SEMID_REGLOG			0x00000004	/**< レジスタログ書込用			*/
#define CMD_SEMID_ASSERTLOG			0x00000005	/**< アサートログ書込用			*/
#define CMD_SEMID_ABORTLOG			0x00000006	/**< アボートログ書込用			*/
#define CMD_SEMID_COUNTLOG			0x00000007	/**< 時系列カウンタ書込用		*/
#define CMD_SEMID_ALMCTL			0x00000008	/**< for alarm report/clear		*/
#define CMD_SEMID_SVCTL				0x00000009	/* for SV ctrl control set 		*/
#define CMD_SEMID_SPI				0x0000000A	/**< for SPI use*				*/
#define CMD_SEMID_TCPIP				0x0000000B	/* for ARTM LAN					*/
#define CMD_SEMID_PPC405			0x0000000C	/* for PPC405					*/
#define CMD_SEMID_ERRSET			0x0000000D	/* for soft ERR control set 	*/
#define CMD_SEMID_RXREG				0x0000000E	/* for RX register access 		*/
#define CMD_SEMID_L2CACS			0x0000000F	/* L2Cデバイスアクセス @800M 	*/
#define CMD_SEMID_SNSFLGACS			0x00000010	/* センサアクセスフラグアクセス @800M */

#define CMD_SEMID_FLASH				0x00000011
#define CMD_SEMID_CARDCNT			0x00000012

/* ======================================================================== */
/* タイマモード定数定義 													*/
/* ======================================================================== */
#define CMD_TIMMD_SINGLE			0x00000001	/**< シングルモード(NON-RESET)	*/
#define CMD_TIMMD_REPEAT			0x00000002	/**< リピートモード(NON-RESET)	*/
#define CMD_TIMMD_SINGLE_R			0x00000011	/**< シングルモード(RESET)		*/
#define CMD_TIMMD_REPEAT_R			0x00000012	/**< リピートモード(RESET)		*/


/* ======================================================================== */
/* システムクロック値定義													*/
/* ======================================================================== */
#define CMD_MSEC				1			/**< システムクロック1ms		*/


/* ======================================================================== */
/* タイマ値計算式定義														*/
/* ======================================================================== */
#define TIMVAL_GET( timval ) 	((UINT)( (timval) * 10 / CMD_MSEC ))


/* ======================================================================== */
/* ログ取得用定数定義														*/
/* ======================================================================== */
/* テーブル配列数 */
#define CMD_LOG_ENTERNUM		512			/**< タスク間通信ログ用配列数		*/
#define CMD_LOG_BUFHISTLOGNUM	512			/**< バッファ操作履歴ログ用配列数	*/
#define CMD_LOG_BUFMNGLOGNUM	512			/**< バッファ未返却ログ用配列数		*/
#define CMD_LOG_TIMHISTLOGNUM	512			/**< タイマ操作履歴ログ用配列数		*/
#define CMD_LOG_TIMMNGLOGNUM	4321		/**< タイマ状態管理ログ用配列数		*/
#define CMD_LOG_REGHISTLOGNUM	1024		/**< レジスタ操作履歴ログ用灰列数	*/
#define CMD_LOG_ASSERTLOGNUM	512			/**< アサートログ用配列数			*/

/* ログ種別 */
#define CMD_LOGKIND_TSKLOG		0xDD11		/**< タスク間通信ログ用			*/
#define CMD_LOGKIND_BUFHISTLOG	0xDD22		/**< バッファ操作履歴ログ用		*/
#define CMD_LOGKIND_BUFMNGLOG	0xDD33		/**< バッファ未返却管理ログ		*/
#define CMD_LOGKIND_TIMHISTLOG	0xDD44		/**< タイマ操作履歴ログ用		*/
#define CMD_LOGKIND_TIMMNGLOG	0xDD55		/**< タイマ状態管理ログ用 		*/
#define CMD_LOGKIND_REGWRTLOG	0xDD66		/**< レジスタ書込履歴ログ用		*/
#define CMD_LOGKIND_ASSERTLOG	0xDD77		/**< アサートログ用				*/
#define CMD_LOGKIND_ABORTLOG	0xDD88		/**< アボートログ用				*/
#define CMD_LOGKIND_RALOG		0xDD99		/**< リソースアラームログ用		*/

/* 送受信種別 */
#define CMD_MSG_SNDLOG			0x1111		/**< 送信ログ					*/
#define CMD_MSG_RCVLOG			0x2222		/**< 受信ログ					*/

/* 操作種別 */
#define CMD_CTRL_ON				0x1111		/**< 起動・取得					*/
#define CMD_CTRL_OFF			0x2222		/**< 停止・返却					*/
#define CMD_CTRL_TO				0x3333		/**< T.O.発生					*/

/* タイマ状態 */
#define CMD_TIMERSTS_ON			0x1111		/**< タイマ起動					*/
#define CMD_TIMERSTS_OFF		0x2222		/**< タイマ停止					*/
#define CMD_TIMERSTS_TO			0x3333		/**< タイマTO					*/

/* データサイズ */
#define CMD_TSKLOG_DATSIZE		32			/**< タスク間通信ログデータ部Size	*/
#define CMD_ASSERT_MSGSIZE_MAX	32			/**< アサートメッセージ最大Size		*/

#define CMD_RTC_SET_REG1		0xF10000C0	/**< 時・分情報					*/
#define CMD_RTC_SET_REG2		0xF10000C2	/**< 秒・ms情報					*/
#define CMD_RTC_SET_REG3		0xF10000C4	/**< 年情報						*/
#define CMD_RTC_SET_REG4		0xF10000C6	/**< 月・日情報					*/

/* FLASHアクセステーブル関連 */
#define CMD_FL_TBL_MEMBER0		0			/**< FLASHアクセステーブルメンバ0 */
#define CMD_FL_TBL_MEMBER1		1			/**< FLASHアクセステーブルメンバ1 */
#define CMD_FL_TBL_MEMBER2		2			/**< FLASHアクセステーブルメンバ2 */
#define CMD_FL_TBL_MEMBER3		3			/**< FLASHアクセステーブルメンバ3 */
#define CMD_FL_TBL_MEMBER4		4			/**< FLASHアクセステーブルメンバ4 */
#define CMD_FL_TBL_MEMBER5		5			/**< FLASHアクセステーブルメンバ5 */
#define CMD_FL_TBL_MAX_ARY		22			/**< FLASHアクセステーブル配列数	*/

#define CMD_MAX_SEC_NUM_CPRIFPGA	128		/**< MAXセクタ数(CPRI-FPGA)			*/
#define CMD_MAX_SEC_NUM_FIRM		56		/**< MAXセクタ数(TRIF-FPGA用)		*/
#define CMD_MAX_SEC_NUM_ALM			8		/**< MAXセクタ数(ALARM LOG用)		*/
#define CMD_MAX_SEC_NUM_BOOT		8		/**< MAXセクタ数(BOOT 用)			*/
#define CMD_MAX_SEC_NUM_TP			8		/**< MAXセクタ数(TP 用)				*/
#define CMD_MAX_SEC_NUM				CMD_MAX_SEC_NUM_CPRIFPGA/**< MAXセクタ数	*/

#define	CMD_SEC_SIZE				0x20000 /**< 1 Sector size 					*/

/* ======================================================================== */
/* レジスタ制御関連定義														*/
/* ======================================================================== */

/* EEPROMアドレス制御レジスタ アイドル状態 */
#define CMD_EEPROM_ACS_OK		0			/**< アクセス可能状態 */
#define CMD_EEPROM_IDL			1			/**< アイドル中 */

/* EEPROMアドレス制御レジスタ 制御コード */
#define CMD_EEPROM_OPCD_WRITE	0x01		/**< OP-CODE:ライト */
#define CMD_EEPROM_OPCD_READ	0x02		/**< OP-CODE:リード */

/* EEPROMアドレス制御レジスタ アドレスデータ */
#define CMD_EEPROM_ADDT_WRIDIB	0x0300		/**< 書き込み許可 */
#define CMD_EEPROM_ADDT_WRIENB	0x0000		/**< 書き込み禁止 */

/* ================================================================================ */
/* EEPROM制御レジスタ(for TP)														*/
/* ================================================================================ */
#define	CMD_EEP_MAX				0x400			/**< 終端アドレス					*/
#define	CMD_EEP_SIZE			0x400			/**< サイズ							*/
#define CMD_REGADR_EEP_VER		0x7F			/**< EEPROMバージョン				*/
	
/* ======================================================================== */
/* ポーリング処理関連定義													*/
/* ======================================================================== */
#define CMD_TIMING_LIM_OVER		1			/**< タイミングリミット超過		*/
#define CMD_CHKSUM_BLK_SIZE		4			/**< チェックサムブロック単位		*/


/* ======================================================================== */
/* CPRIリンク番号定義														*/
/* ======================================================================== */
#define CMD_CPRINO_01			1			/**< CPRI# 1リンク番号			*/
#define CMD_CPRINO_02			2			/**< CPRI# 2リンク番号			*/
#define CMD_CPRINO_03			3			/**< CPRI# 3リンク番号			*/
#define CMD_CPRINO_04			4			/**< CPRI# 4リンク番号			*/
#define CMD_CPRINO_05			5			/**< CPRI# 5リンク番号			*/
#define CMD_CPRINO_06			6			/**< CPRI# 6リンク番号			*/
#define CMD_CPRINO_07			7			/**< CPRI# 7リンク番号			*/
#define CMD_CPRINO_08			8			/**< CPRI# 8リンク番号			*/
#define CMD_CPRINO_09			9			/**< CPRI# 9リンク番号			*/
#define CMD_CPRINO_10			10			/**< CPRI#10リンク番号			*/
#define CMD_CPRINO_11			11			/**< CPRI#11リンク番号			*/
#define CMD_CPRINO_12			12			/**< CPRI#12リンク番号			*/
#define CMD_CPRINO_13			13			/**< CPRI# 3リンク番号			*/
#define CMD_CPRINO_14			14			/**< CPRI# 4リンク番号			*/
#define CMD_CPRINO_15			15			/**< CPRI# 5リンク番号			*/
#define CMD_CPRINO_16			16			/**< CPRI# 6リンク番号			*/
#define CMD_CPRINO_17			17			/**< CPRI# 7リンク番号			*/
#define CMD_CPRINO_18			18			/**< CPRI# 8リンク番号			*/
#define CMD_CPRINO_19			19			/**< CPRI# 9リンク番号			*/
#define CMD_CPRINO_20			20			/**< CPRI#10リンク番号			*/
#define CMD_CPRINO_21			21			/**< CPRI#11リンク番号			*/
#define CMD_CPRINO_22			22			/**< CPRI#12リンク番号			*/
#define CMD_CPRINO_23			23			/**< CPRI# 3リンク番号			*/
#define CMD_CPRINO_24			24			/**< CPRI# 4リンク番号			*/

/* ======================================================================== */
/* AMP番号定義																*/
/* ======================================================================== */
#define CMD_AMPNO_1				1			/**< AMP#1番号					*/
#define CMD_AMPNO_2				2			/**< AMP#2番号					*/
#define CMD_AMPNO_MAX			2			/**< AMP最大数					*/

/* ======================================================================== */
/* OF-TRX番号定義															*/
/* ======================================================================== */
#define CMD_OFTRXNO_01			1			/**< OF-TRX# 1					*/
#define CMD_OFTRXNO_02			2			/**< OF-TRX# 2					*/
#define CMD_OFTRXNO_03			3			/**< OF-TRX# 3					*/
#define CMD_OFTRXNO_04			4			/**< OF-TRX# 4					*/
#define CMD_OFTRXNO_05			5			/**< OF-TRX# 5					*/
#define CMD_OFTRXNO_06			6			/**< OF-TRX# 6					*/
#define CMD_OFTRXNO_07			7			/**< OF-TRX# 7					*/
#define CMD_OFTRXNO_08			8			/**< OF-TRX# 8					*/
#define CMD_OFTRXNO_09			9			/**< OF-TRX# 9					*/
#define CMD_OFTRXNO_10			10			/**< OF-TRX#10					*/
#define CMD_OFTRXNO_11			11			/**< OF-TRX#11					*/
#define CMD_OFTRXNO_12			12			/**< OF-TRX#12					*/
#define CMD_OFTRXNO_13			13			/**< OF-TRX#13					*/
#define CMD_OFTRXNO_14			14			/**< OF-TRX#14					*/
#define CMD_OFTRXNO_15			15			/**< OF-TRX#15					*/
#define CMD_OFTRXNO_16			16			/**< OF-TRX#16					*/
#define CMD_OFTRXNO_17			17			/**< OF-TRX#17					*/
#define CMD_OFTRXNO_18			18			/**< OF-TRX#18					*/
#define CMD_OFTRXNO_19			19			/**< OF-TRX#19					*/
#define CMD_OFTRXNO_20			20			/**< OF-TRX#20					*/
#define CMD_OFTRXNO_21			21			/**< OF-TRX#21					*/
#define CMD_OFTRXNO_22			22			/**< OF-TRX#22					*/
#define CMD_OFTRXNO_23			23			/**< OF-TRX#23					*/
#define CMD_OFTRXNO_24			24			/**< OF-TRX#24					*/
#define CMD_OFTRXNO_MAX			24			/**< OFTRX最大数					*/

/* ======================================================================== */
/* 外部装置番号定義															*/
/* ======================================================================== */
#define CMD_EXT_OARAIFN1		1			/**< OA-RA-INF1					*/
#define CMD_EXT_OARAINF2		2			/**< OA-RA-INF2					*/
#define CMD_EXT_MTTINF			4			/**< MTT-INF						*/
#define CMD_EXT_TILTINF			3			/**< TILT-INF						*/
#define CMD_EXT_KINDMAX			4			/**< 外部装置種別最大数			*/

/* ======================================================================== */
/* スロット番号定義															*/
/* ======================================================================== */
#define CMD_SLTNO_01			1			/**< スロット# 1番号				*/
#define CMD_SLTNO_02			2			/**< スロット# 2番号				*/
#define CMD_SLTNO_03			3			/**< スロット# 3番号				*/
#define CMD_SLTNO_04			4			/**< スロット# 4番号				*/
#define CMD_SLTNO_05			5			/**< スロット# 5番号				*/
#define CMD_SLTNO_06			6			/**< スロット# 6番号				*/
#define CMD_SLTNO_07			7			/**< スロット# 7番号				*/
#define CMD_SLTNO_08			8			/**< スロット# 8番号				*/
#define CMD_SLTNO_09			9			/**< スロット# 9番号				*/
#define CMD_SLTNO_10			10			/**< スロット#10番号				*/
#define CMD_SLTNO_11			11			/**< スロット#11番号				*/
#define CMD_SLTNO_12			12			/**< スロット#12番号				*/
#define CMD_SLTNO_13			13			/**< スロット#13番号				*/
#define CMD_SLTNO_14			14			/**< スロット#14番号				*/
#define CMD_SLTNO_15			15			/**< スロット#15番号				*/
#define CMD_SLTNO_16			16			/**< スロット#16番号				*/
#define CMD_SLTNO_17			17			/**< スロット#17番号				*/
#define CMD_SLTNO_18			18			/**< スロット#18番号				*/
#define CMD_SLTNO_19			19			/**< スロット#19番号				*/
#define CMD_SLTNO_20			20			/**< スロット#20番号				*/
#define CMD_SLTNO_21			21			/**< スロット#21番号				*/
#define CMD_SLTNO_22			22			/**< スロット#22番号				*/
#define CMD_SLTNO_23			23			/**< スロット#23番号				*/
#define CMD_SLTNO_24			24			/**< スロット#24番号				*/
#define CMD_SLTNO_25			25			/**< スロット#25番号				*/
#define CMD_SLTNO_26			26			/**< スロット#26番号				*/
#define CMD_SLTNO_27			27			/**< スロット#27番号				*/
#define CMD_SLTNO_28			28			/**< スロット#28番号				*/
#define CMD_SLTNO_29			29			/**< スロット#29番号				*/
#define CMD_SLTNO_30			30			/**< スロット#30番号				*/
#define CMD_SLTNO_31			31			/**< スロット#31番号				*/
#define CMD_SLTNO_32			32			/**< スロット#32番号				*/
#define CMD_SLTNO_33			33			/**< スロット#33番号				*/
#define CMD_SLTNO_34			34			/**< スロット#34番号				*/
#define CMD_SLTNO_35			35			/**< スロット#35番号				*/
#define CMD_SLTNO_36			36			/**< スロット#36番号				*/
#define CMD_SLTNO_37			37			/**< スロット#37番号				*/
#define CMD_SLTNO_38			38			/**< スロット#38番号				*/
#define CMD_SLTNO_39			39			/**< スロット#39番号				*/
#define CMD_SLTNO_40			40			/**< スロット#40番号				*/
#define CMD_SLTNO_41			41			/**< スロット#41番号				*/
#define CMD_SLTNO_42			42			/**< スロット#42番号				*/
#define CMD_SLTNO_43			43			/**< スロット#43番号				*/
#define CMD_SLTNO_44			44			/**< スロット#44番号				*/
#define CMD_SLTNO_45			45			/**< スロット#45番号				*/
#define CMD_SLTNO_46			46			/**< スロット#46番号				*/
#define CMD_SLTNO_47			47			/**< スロット#47番号				*/
#define CMD_SLTNO_48			48			/**< スロット#48番号				*/
#define CMD_SLTNO_49			49			/**< スロット#49番号				*/
#define CMD_SLTNO_50			50			/**< スロット#50番号				*/
#define CMD_SLTNO_51			51			/**< スロット#51番号				*/
#define CMD_SLTNO_52			52			/**< スロット#52番号				*/
#define CMD_SLTNO_53			53			/**< スロット#53番号				*/
#define CMD_SLTNO_54			54			/**< スロット#54番号				*/
#define CMD_SLTNO_55			55			/**< スロット#55番号				*/
#define CMD_SLTNO_56			56			/**< スロット#56番号				*/
#define CMD_SLOTNO_MAX			56			/**< スロット番号最大数			*/


/* ======================================================================== */
/* 障害ログ関連定義				 											*/
/* ======================================================================== */
#define CMD_ALMLOG_ENABLE	0x77777777	/**< 障害ログデータ有効値			*/

#define CMD_RTMOS_AREASIZE	0x60			/**< RTMOSサイズ(512)				*/

#define CMD_FLSADR_ALMLOGBNK0	0xF9000000	/**< 障害ログ用 BANK#1アドレス	*/
#define CMD_FLSADR_ALMLOGBNK1	0xF9100000	/**< 障害ログ用 BANK#2アドレス	*/
#define CMD_FLSADR_ALMLOGBNK2	0xF9200000	/**< 障害ログ用 BANK#3アドレス	*/
#define CMD_FLSADR_ALMLOGBNK3	0xF9300000	/**< 障害ログ用 BANK#4アドレス	*/
#define CMD_FLSADR_ALMLOGBNK4	0xF9400000	/**< 障害ログ用 BANK#5アドレス	*/
#define CMD_FLSADR_ALMLOGBNK5	0xF9500000	/**< 障害ログ用 BANK#6アドレス	*/
#define CMD_FLSADR_ALMLOGBNK6	0xFB000000	/**< 障害ログ用 BANK#7アドレス	*/
#define CMD_FLSADR_ALMLOGBNK7	0xFB100000	/**< 障害ログ用 BANK#8アドレス	*/
#define CMD_FLSADR_ALMLOGBNK8	0xFB200000	/**< 障害ログ用 BANK#9アドレス	*/
#define CMD_FLSADR_ALMLOGBNK9	0xFB300000	/**< 障害ログ用 BANK#10アドレス	*/
#define CMD_FLSADR_ALMLOGBNK10	0xFB400000	/**< 障害ログ用 BANK#11アドレス	*/
#define CMD_FLSADR_ALMLOGBNK11	0xFB500000	/**< 障害ログ用 BANK#12アドレス	*/

#define CMD_ALMLOG_BNK0SIDE		0			/**< 障害ログ用FLASH-ROM BANK#0	*/
#define CMD_ALMLOG_BNK1SIDE		1			/**< 障害ログ用FLASH-ROM BANK#1	*/
#define CMD_ALMLOG_BNK2SIDE		2			/**< 障害ログ用FLASH-ROM BANK#2	*/
#define CMD_ALMLOG_BNK3SIDE		3			/**< 障害ログ用FLASH-ROM BANK#3	*/
#define CMD_ALMLOG_BNK4SIDE		4			/**< 障害ログ用FLASH-ROM BANK#4	*/
#define CMD_ALMLOG_BNK5SIDE		5			/**< 障害ログ用FLASH-ROM BANK#5	*/
#define CMD_ALMLOG_BNK6SIDE		6			/**< 障害ログ用FLASH-ROM BANK#6	*/
#define CMD_ALMLOG_BNK7SIDE		7			/**< 障害ログ用FLASH-ROM BANK#7	*/
#define CMD_ALMLOG_BNK8SIDE		8			/**< 障害ログ用FLASH-ROM BANK#8	*/
#define CMD_ALMLOG_BNK9SIDE		9			/**< 障害ログ用FLASH-ROM BANK#9	*/
#define CMD_ALMLOG_BNK10SIDE	10			/**< 障害ログ用FLASH-ROM BANK#10	*/
#define CMD_ALMLOG_BNK11SIDE	11			/**< 障害ログ用FLASH-ROM BANK#11	*/
#define CMD_ALMLOG_BNKMAXSIDE	12			/**< 障害ログ用FLASH-ROM BANK count*/
/* Phase1としては障害ログの1面の有効サイズは385232byte固定であるので、		*/
/* 下記デファインを使用しているが、今後共通テーブルサイズ変更や				*/
/* 障害ログで取得するデータ変更がある場合は、デファイン値を見直す必要あり	*/
#define CMD_ALMLOG_INP_SIZE		385232		/**< 障害ログ重要ログサイズ(Pase1)*/

#define CMD_LOGKIND_NOM			0			/**< 空き							*/
#define CMD_LOGKIND_MEM			1			/**< メモリ情報設定				*/
#define CMD_LOGKIND_RTM			2			/**< RTM情報設定					*/
#define CMD_LOGKIND_REG			3			/**< レジスタ情報設定				*/
#define CMD_LOGKIND_ROM			4			/**< EEPROM情報設定				*/
#define CMD_LOGKIND_ALM			3			/**< レジスタ情報設定				*/
#define CMD_LOGKIND_MPU			5			/**< レジスタ情報設定				*/
#define CMD_LOGKIND_CON			16			/**< 重要ログと通常ログの切れ目	*/
#define CMD_LOGKIND_END			255			/**< 終了							*/

#define CMD_LOG_SEPANUM			4			/**< セパレータ配列数				*/
#define CMD_ALMLOG_NUM			60			/**< 障害ログ設定テーブル配列数	*/
#define CMD_ALMLOG_REG_NUM		44			/**< collected Register number for Alarm LOG	*/
#define CMD_MPULOG_REG_NUM		64			/**< collected Register number for Alarm LOG	*/

/* ======================================================================== */
/* ダウンロード関連定義				 										*/
/* ======================================================================== */
	/* ZTX_ZTX   */
#define CMD_RAM_BUFSIZE1 (8 + 8 + CMD_FLDCMD_MXLEN + 16) /**< RAM size for MDIF(last 16bytes is reserved) */
#define CMD_RAM_BUFNUM1				1		/**< Amount of ram buffer kind 1  */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_BUFCA_RAM_UPLNK 		E_BPF_RU_IPCM_BUFF_KIND_MTMMLR		/**< RAM category: Uplink(FLD-MT data command RX) */
#define CMD_BUFCA_RAM_DWNLNK		E_BPF_RU_IPCM_BUFF_KIND_MTMMLR		/**< RAM category: Downlink(FLD-MT data command TX) */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_EEPSIDINF0			0x0000			/**< EEPROM起動面０(0x0000)*/
#define CMD_EEPSINFMSK1			0x0001			/**< EEPROM起動面情報マスク(0x0001)*/

#define CMD_REFILENAME_SIZE		20			/**< REファイル名サイズ(byte)		*/
#define CMD_CHECKSUM_VALUE		0xFFFF		/**< チェックサム繰り上がり判定値		*/

/* ======================================================================== */
/* 新旧OF-TRX種別(mdeif_comから移動)										*/
/* ======================================================================== */
#define OLD_OFTRX				0			/**< 現行OF-TRX					*/
#define NEXT_OFTRX				0x8000		/**< 次期OF-TRX(VA対応)			*/

/* ======================================================================== */
/* バルクダウン版FPGA(EP2S60)識別(mdeif_comから移動)						*/
/* ======================================================================== */
#define BD_NOYTPE				0			/**< 非バルクダウンタイプ			*/
#define BD_TYTE					0x4000		/**< バルクダウンタイプ			*/

/* ======================================================================== */
/* LSI(ASIC)対応装置識別(mdeif_comから移動)									*/
/* ======================================================================== */
#define LSI_NOYTPE				0			/**< LSI[eASIC]対応装置			*/
#define LSI_TYTPE				0x2000		/**< 非LSI[eASIC]対応装置			*/

/* ======================================================================== */
/* 診断関連定義				 												*/
/* ======================================================================== */
#define CMD_CHKSUMSIZE_BOOT	 	0x80000			/**< BOOT領域サイズ			*/
#define CMD_CHKSUM_EXP			0xffffffff		/**< チェックサム値			*/

/* ======================================================================== */
/* WDT関連レジスタ															*/
/* ======================================================================== */
#define CMD_REGADR_WDT			0xF10000B0	/**< WDT制御					*/
#define WDD_REG_NMSK			0x03FF			/**< 未使用ビットマスク値	*/
#define WDD_WDT_CLR				0x0200			/**< タイマークリア			*/

/* ======================================================================== */
/* デバイス番号																*/
/* ======================================================================== */
#define CMD_DEVICENO_ONLINE			2			/**< オンラインデバッガ		*/
#define CMD_DEVICENO_OFFLINE		1			/**< オフラインデバッガ		*/

/* ======================================================================== */
/* DBGコマンド関連															*/
/* ======================================================================== */
/* DBG種別 */
#define CMD_DBG_KND_DMP_ALL			0x0101		/**< ダンプ(全表示)			*/
#define CMD_DBG_KND_DMP_TSK			0x0102		/**< ダンプ(タスク別表示)	*/
#define CMD_DBG_KND_DMP_TBL			0x0103		/**< ダンプ(テーブル別表示)	*/
#define CMD_DBG_KND_DMP_VER			0x0104		/**< ダンプ(バージョン表示)	*/
#define CMD_DBG_KND_DMP_REG			0x0105		/**< ダンプ(レジスタ表示)	*/
#define CMD_DBG_KND_DMP_AST			0x0106		/**< ダンプ(Assert表示)		*/
#define CMD_DBG_KND_DMP_ENT			0x0107		/**< ダンプ(Enter表示)		*/
#define CMD_DBG_KND_DMP_RGH			0x0108		/**< ダンプ(RegHistLog表示)	*/
#define	CMD_DBG_KND_DMP_TBL2		0x0111		/**< ダンプ(テーブル別表示2)	*/

#define CMD_DBG_KND_DPL_ALL			0x0201		/**< リスト(全表示)			*/
#define CMD_DBG_KND_DPL_TSK			0x0202		/**< リスト(タスク別表示)	*/
#define CMD_DBG_KND_DPL_TBL			0x0203		/**< リスト(テーブル別表示)	*/
#define	CMD_DBG_KND_DPL_TBL2		0x0211		/**< リスト(テーブル別表示2)	*/

#define CMD_DBG_KND_TSK				0x0300		/**< タスク個別関数			*/

#define CMD_DBG_KND_CPRI_MT			0x0400		/**< CPRI-MT接続用			*/

#define	CMD_DBG_KND_STA_AST			0x0506		/**< RESTART(AssertLog)		*/
#define	CMD_DBG_KND_STA_ENT			0x0507		/**< RESTART(EnterLog)		*/
#define	CMD_DBG_KND_STA_RGH			0x0508		/**< RESTART(RegHistLog)	*/
#define	CMD_DBG_KND_STA_TIM			0x0509		/**< RESTART(timhistlog)	*/
#define	CMD_DBG_KND_STA_BUF			0x050A		/**< RESTART(bufhistlog)	*/

#define CMD_DBG_KND_PRM_ERR			0x9999		/**< パラメータエラー		*/

#define CMD_DBG_DMP_SIZE_MAX		0x00100000	/**< 表示テーブルMAXサイズ	*/
#define CMD_DBG_VER_ADR				0x00003010	/**< バージョン表示アドレス	*/
#define CMD_DBG_VER_ADR_SIZE		0x00000100	/**< バージョン表示サイズ	*/
#define CMD_DBG_REG_ADR				0x30000000	/**< レジスタトップアドレス	*/
#define CMD_DBG_REG_ADR_SIZE		0x00005000	/**< レジスタトップサイズ	*/

/* ======================================================================== */
/* カード状態チェック処理 ( m_cm_ChkCardSta )								*/
/* ======================================================================== */
/** @name check Card status
 *  @{
 */

/* 対象種別 */
#define CMD_TARGET_SLOT			0			/**< スロット番号指定			*/
#define CMD_TARGET_CARD			1			/**< カード種別指定				*/

/* 判定対象 (m_cm_GetCardSta共通) */
#define CMD_TRA					0			/**< TRAカード					*/

/* チェック状態 */
#define CMD_CHK_INS				0x0080		/**< 実装							*/
#define CMD_CHK_NOINS			0x0040		/**< 未実装							*/
#define CMD_CHK_CONST			0x0001		/**< カード工事中状態				*/
#define CMD_CHK_ACT				0x0020		/**< ACT							*/
#define CMD_CHK_USE				0x0400		/**< USE							*/
#define CMD_CHK_ALM				0x0200		/**< ALM							*/
#define CMD_CHK_ERR				0x0100		/**< ERR							*/
#define CMD_CHK_BLK				0x0004		/**< BLK							*/
#define CMD_CHK_TST				0x0002		/**< CHK							*/


/* チェック結果	*/
#define CMD_CHKCARDSTA_R_OK			0	/**< Card status is the same as specified status	*/
#define CMD_CHKCARDSTA_R_NG			1	/**< Card status is the same as specified status	*/

#define CMD_CHKBRSTA_R_SAME		1   /*same as check*/
#define CMD_CHKBRSTA_R_NOSAME		0   /*not same as check*/

/** @} */

/* ======================================================================== */
/* BR状態チェック処理 ( m_cm_ChkBrxSta )								*/
/* ======================================================================== */
/** @name check BR status
 *  @{
 */


/* 判定対象 (CMD_BR0, CMD_BR1) */

/* チェック状態 (m_cm_ChkCardSta共通)*/

/* チェック結果	*/
#define CMD_CHKBRSTA_R_OK				0	/**< BR status is the same as specified status */
#define CMD_CHKBRSTA_R_ELSENG			1	/**< R status is others NG case			*/

/* #define CMD_CHKBRSTA_R_NOUSE			2 */	/**< BR status is not USE					*/
/* #define CMD_CHKBRSTA_R_NOALM			3 */	/**< BR status is not ALARM				*/
/* #define CMD_CHKBRSTA_R_NOERR			x */	/**< BR status is not ERR				*/

/** @} */

/* ======================================================================== */
/* カード検索処理 ( m_cm_CardSarch )										*/
/* ======================================================================== */
/* Return値		*/
#define CMD_CARDSARCH_R_BRE		0			/**< BRE card					*/
#define CMD_CARDSARCH_R_RRE		1			/**< RRE card					*/
#define CMD_CARDSARCH_R_LRE		2			/**< LRE card				*/

/* ======================================================================== */
/* Activeチェック処理 ( m_cm_ActiveChk )									*/
/* ======================================================================== */
/* Return値		*/
#define CMD_STATE_ACTIVE		0			/**< Active状態					*/
#define CMD_STATE_NEGATIVE		1			/**< Negative状態				*/

/* ======================================================================== */
/* シリアルバス設定処理 ( m_cm_SerialBasSet )								*/
/* ======================================================================== */

/* Branch No :  CMD_BR0, CMD_BR1 */


/* チェック要否	*/
#define CMD_CHK_NCY				1			/**< R/Wチェック要				*/
#define CMD_CHK_UNNCY			0			/**< R/Wチェック不要			*/

/* ======================================================================== */
/* First diagnosis processing ( m_cm_1stDia )								*/
/* ======================================================================== */
/** @name check pattern number
 *  @{
 */

/****************************************************************************/
/* check pattern number for 1st diagnosis									*/
/****************************************************************************/
#define CMD_MEM_CHK_PTNNUM		2		/**< Memory check pattern number	*/
#define CMD_REG_CHK_PTNNUM		2		/**< register check pattern number	*/

/** @} */

/* ======================================================================== */
/* Get GPIO bit processing ( m_cm_ReadGPIOBit )								*/
/* ======================================================================== */
/** @name Get GPIO bit data
 *  @{
 */

/****************************************************************************/
/* bit data of GPIO register									*/
/****************************************************************************/
#define CMD_GPIO_BIT_0		0		/**< bit dat is 0		*/
#define CMD_GPIO_BIT_1		1		/**< bit dat is 1		*/

/** @} */

/* ======================================================================== */
/* ERR control register set processing( m_cm_ErrCntRegSet )					*/
/* ======================================================================== */
/** @name Set ERR control register
 *  @{
 */

/****************************************************************************/
/* Card ALM check need or not(m_cm_ErrCntRegSet)							*/
/****************************************************************************/
#define CMD_CHKALM_NCY		1		/**< Need check */
#define CMD_CHKALM_UNNCY	0		/**< No need check */

/** @} */


/*!  
 * @name SET/CLEAR action define
 * @{  
 */
#define CMD_SET				1
#define CMD_CLR				0

/* @} */



/** @name Diagnosis Check pattern data
 *  @{
 */

/****************************************************************************/
/* Mian memory check pattern data											*/
/****************************************************************************/
/* the pattern data is TBD */

#define CMD_MEM_CHK_PTN_AAAA		0xAAAA	/**< memory Check pattern data 1 */
#define CMD_MEM_CHK_PTN_5555		0x5555	/**< memory Check pattern data 2 */


/****************************************************************************/
/* CPRI FPGA Register check pattern data									*/
/****************************************************************************/
#define CMD_REG_CHK_PTN_AAAA		0xAAAA	/**< Register Check pattern data 1 */
#define CMD_REG_CHK_PTN_5555		0x5555	/**< Register Check pattern data 2 */
#define CMD_REG_CHK_PTN_AAAAAAAA	0xAAAAAAAA	/**< Register Check pattern data 1 */
#define CMD_REG_CHK_PTN_55555555	0x55555555	/**< Register Check pattern data 2 */


/** @} */

/* ======================================================================== */
/* CPRI stateF 制御レジスタ								(R/W)	*/
/* ======================================================================== */
#define CMD_T4_TIMER			0x0064	/**< T4 timer default value (10s) */




/* ======================================================================== */
/* マスクパターン 															*/
/* ======================================================================== */
#define CMD_TOFFSET_MSK			0x00FF		/**< Toffset値マスク			*/
#define CMD_DL_DELAY_MSK		0x00FF		/**< 下り装置内遅延値マスク		*/
#define CMD_UL_DELAY_MSK		0x00FF		/**< 上り装置内遅延値マスク		*/


/* ======================================================================== */
/* マスクパターン 															*/
/* ======================================================================== */
/* E-TYP */
#define CMD_ETYP_MSK			0x0001		/**< E-TYPタイプマスク			*/
/* P-TYP */
#define CMD_PTYP_MSK			0x0006		/**< P-TYPタイプマスク			*/
/* DIV  */
#define CMD_DIV_MSK				0x0008		/**< DIVマスク					*/

/* ======================================================================== */
/* ERR制御レジスタ															*/
/* ======================================================================== */
#define CMD_REGADR_ERR_CNT		0x90000318	/**< CPRIリンクERR設定レジスタ	*/

/* マスクパターン */
/******************/
/* CPRIリンクERR設定レジスタ	*/
#define CMD_ERR_CNT_L1ERR			0x0001		/**< L1 ERR						*/
#define CMD_ERR_CNT_L2ERR			0x0001		/**< L2 ERR						*/
#define CMD_ERR_CNT_L3ERR_3G		0x0002		/**< L3 ERR for 3G				*/
#define CMD_ERR_CNT_L3ERR_S3G		0x0004		/**< L3 ERR for S3G				*/
#define CMD_ERR_CNT_CPSL3ERR_S3G	0x00200000		/**< CPS L3 ERR for S3G			*/
#define CMD_ERR_CNT_L2OFF			0xfffe		/**< L2 ERR off					*/
#define CMD_ERR_CNT_L3OFF_3G		0xfffd		/**< L3 ERR off for 3G			*/
#define CMD_ERR_CNT_L3OFF_S3G		0xfffb		/**< L3 ERR off for S3G			*/
#define CMD_ERR_CNT_CPSL3OFF_S3G	0xffdfffff		/**< CPS L3 ERR off for S3G		*/

/* ソフトERR制御(基本A7)		*/
#define CMD_ERR12_BR0_SIR_NG			0x0010		/**< TRA Br0シリアル制御NG		*/
#define CMD_ERR13_BR1_SIR_NG			0x0020		/**< TRA Br1シリアル制御NG		*/
#define CMD_ERR14_EXT_SIR_NG			0x0040		/**< EXTシリアル制御NG			*/

/* ソフトERR制御(基本A8)		*/
#define CMD_ERR16_BR0_SIR_NG			0x0001		/**< 2次診断TRABr0シリアル制御NG	*/
#define CMD_ERR17_BR1_SIR_NG			0x0002		/**< 2次診断TRABr1シリアル制御NG	*/
#define CMD_ERR18_BR0_ULIF_NG			0x0004		/**< TRABr0 上り高速IFNG			*/
#define CMD_ERR19_BR1_ULIF_NG			0x0008		/**< TRABr1 上り高速IFNG			*/
#define CMD_ERR20_BR0_DLIF_NG			0x0010		/**< TRABr0 下り高速IFNG			*/
#define CMD_ERR21_BR1_DLIF_NG			0x0020		/**< TRABr1 下り高速IFNG			*/
#define CMD_ERR22_MTT_NG				0x0040		/**< OF時のMTT回線断検出			*/
#define CMD_ERR23_MTT_PAL_NG			0x0040		/**< OF時のMTT回線パリティNG		*/

/* CeNB-F-083-028 FFCS)Xut add start */
#define	CMD_ERR_AMP_DISC_ERR			0x0010		/**< AMPｶﾏERR						*/
#define	CMD_ERR_AMP_DISC_ERR_OFF		0xFFEF		/**< AMPｶﾏERR OFF				*/
#define	CMD_ERR_SLP0_ERR_ON				0x0020		/**< SLP0 ERR ON					*/
#define	CMD_ERR_SLP0_ERR_OFF			0xFFDF		/**< SLP0 ERR OFF					*/
#define	CMD_ERR_SLP1_ERR_ON				0x0040		/**< SLP1 ERR ON					*/
#define	CMD_ERR_SLP1_ERR_OFF			0xFFBF		/**< SLP1 ERR OFF					*/
//TD-SRE
#define	CMD_ERR_SLP2_ERR_ON				0x0080		/**< SLP2 ERR ON					*/
#define	CMD_ERR_SLP2_ERR_OFF			0xFF7F		/**< SLP2 ERR OFF					*/
#define	CMD_ERR_SLP3_ERR_ON				0x0100		/**< SLP3 ERR ON					*/
#define	CMD_ERR_SLP3_ERR_OFF			0xFEFF		/**< SLP3 ERR OFF					*/
//TD-RRE
#define	CMD_ERR_CPSL3ERR				0x0020		/**< CPS3ERR						*/
#define	CMD_ERR_CABLELENERR				0x0002		/**< Cable length ERR				*/



/* CeNB-F-083-028 FFCS)Xut add end */

/* ソフトERR制御(基本A9)		*/
#define CMD_ERR24_SOFT_NG_SET			0x0001		/**< SOFT固有ERR検出時				*/
#define CMD_ERR24_SOFT_NG_REL			0x0000		/**< SOFT固有ERR解消時				*/
#define CMD_ERR_DL_INF_ERR				0x4000		/**< Download infor NG 				*/
#define CMD_ERR_DL_CMP_ERR				0x2000		/**< Download complete NG			*/
#define CMD_ERR_DL_INF_ERR_OFF			0xBFFF		/**< Download infor NG cancel		*/
#define CMD_ERR_DL_CMP_ERR_OFF			0xDFFF		/**< Download complete NG cancel	*/
#define CMD_ERR_DL_INF_S3G_ERR			0x1000		/**< Download infor NG for S3G 		*/
#define CMD_ERR_DL_CMP_S3G_ERR			0x0800		/**< Download complete NG for S3G	*/
#define CMD_ERR_DL_INF_S3G_ERR_OFF		0xEFFF		/**< Download infor NG cancel for S3G	*/
#define CMD_ERR_DL_CMP_S3G_ERR_OFF		0xF7FF		/**< Download complete NG cancel for S3G*/
#define CMD_ERR31_L3CARDCNT_ERR			0x8000		/**< SOFT固有ERR検出時					*/
#define CMD_ERR31_L3CARDCNT_ERR_OFF		0x7FFF		/**< SOFT固有ERR解消時					*/
#define CMD_ERR_PARITY_ERR 				0x0400		/**< parity error 						*/
#define CMD_ERR_PARITY_ERR_0FF 			0xFBFF		/**< parity error cancel 				*/
#define CMD_ERR_ORAR_SV_ERR 			0x0200		/**< OARA line off supervision error	*/
#define CMD_ERR_ORAR_SV_ERR_OFF    		0xFDFF		/**< OARA line off supervision error cancel	*/
#define CMD_ERR_MT_ADDR_ERR 			0x0100		/**< MT address setting error			*/
#define CMD_ERR_MT_ADDR_ERR_OFF    		0xFEFF		/**< MT address setting error cancel	*/
#define CMD_ERR_RF_61MCLK_ERR			0x0080		/* BRE-61MCLK ERR 						*/
#define CMD_ERR_RF_61MCLK_ERR_OFF		0xFF7F		/* BRE-61MCLK ERR cancel 				*/

/* ======================================================================== */
/* TILT/PORT状態情報レジスタ												*/
/* ======================================================================== */
#define CMD_REGBITKIND_PROT_OUT		1			/**< 他系PORT OUT			*/

#define CMD_EEPTILT_CONNECT			1			/**< TILT接続有り			*/
#define CMD_EEPTILT_DISCONNECT		0			/**< TILT接続無し			*/

/* ======================================================================== */
/* HDD_HDLID_IRQ								*/
/* ======================================================================== */
#define HDD_HDLID_IRQ0 2 
#define HDD_HDLID_IRQ1 3
#define HDD_HDLID_IRQ2 4
#define HDD_HDLID_IRQ3 5
#define HDD_HDLID_IRQ4 6
#define HDD_HDLID_IRQ5 7
#define HDD_HDLID_ELSE 8  

/* the size of CPRIFPGA file is 12M */
#define CMD_DEVDAT_CPRIFPGA		0xC00000			/* MDIF-FPGA/OFMSDIF-FPGAサイズ				*/
/* the size of FIRM file is 2M */
#define CMD_DEVDAT_FIRM			0x200000				/* MDIFファームサイズ						*/


/* ======================================================================== */
/* definition related download								*/
/* ======================================================================== */
/* the process type of Writing flash */
#define CMD_WRIT_TYP		0	
/* the process type of Erase flash */
#define CMD_ERASE_TYP		1	
 /* the download type is download in startup state */
#define CMD_STAT_TYP		0	
/* the download type is	 download in use state */
#define CMD_USE_TYP			1	


/****************************************************************************/
/*	- No.8 -																*/
/*					MDIFカード内共通使用デファイン値						*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*	- No.8_1 -			タスクIFのデファイン定義							*/
/****************************************************************************/

/* ======================================================================== */
/* 1. RE共通部active / negative定義 for MDIF RE共通部情報共通管理テーブル	*/
/* 2. 設定類別定義	for MDIF task IF RE共通部リンク再確立通知				*/
/* 3. return code定義  for common function m_cm_ActiveChk					*/
/* 4. return code定義  for common function t_cm_ActiveChk					*/
/* ======================================================================== */
#define CMD_RECLS_ACTSET			0	/**< active設定						*/
#define CMD_RECLS_NEGSET			1	/**< negative設定					*/
#define CMD_RECLS_TRIG_TPACTCNT		10	/**< TPコマンドによるActive設定		*/
#define CMD_RECLS_TRIG_TPNEGCNT		11	/**< TPコマンドによるNegative設定	*/
#define	CMD_RECLS_SEQ_TPACTSETRUN	3	/**< TPコマンドactive設定進行中		*/
#define	CMD_RECLS_SEQ_TPNEGSETRUN	4	/**< TPコマンドnegative設定進行中	*/

/* ======================================================================== */
/*	MT type 定義															*/
/* ======================================================================== */
#define CMD_MTTYPE_FLD				0	/**< FLD MT							*/
#define CMD_MTTYPE_SV				1	/**< SV MT							*/
#define CMD_MTTYPE_MK				2	/**< Maker MT						*/

/* ======================================================================== */
/*	TCP link type 定義			 											*/
/* ======================================================================== */
#define CMD_LINKTYPE_FLDCMD			0	/*TCP link for FLD command data transfer	*/
#define CMD_LINKTYPE_FLDFILE		1	/*TCP link for FLD file data transfer		*/
#define CMD_LINKTYPE_SV				2	/*TCP link for SV data transfer				*/
#define CMD_LINKTYPE_MKSIMU			3	/*TCP link for Maker simulator data transfer*/
#define CMD_LINKTYPE_MKMON			4	/*TCP link for Maker monitor data transfer	*/
#define CMD_LINKTYPE_RE				5	/*TCP link for RE data transfer				*/
#define CMD_LINKTYPE_FLDCMD_S3G		6	/*TCP link for S3G FLD command data transfer*/
#define CMD_LINKTYPE_FLDFILE_S3G	7	/*TCP link for S3G FLD file data transfer	*/
#define CMD_LINKTYPE_SV_S3G			8	/*TCP link for S3G SV data transfer			*/
#define CMD_LINKTYPE_FTPCMD			9	/*TCP link for ftp command data transfer	*/
#define CMD_LINKTYPE_FTPFIL			10	/*TCP link for ftp file data transfer		*/


/* ======================================================================== */
/*						  		CPM TIMER EVENT MASK						*/
/* ======================================================================== */
#define CMD_TERMSK_TIM1			0x0002

/* ======================================================================== */
/* MAX CPRI number define in one AMP										*/
/* ======================================================================== */
#define CMD_MX_AMPCPRI			6		/**< MAX CPRI number in one AMP		*/


/* ======================================================================== */
/* ベンダーDefine(ファーム管理テーブル用)									*/
/* ======================================================================== */
#define CMD_VEND_FUJITSU		0			/**< 富士通						*/

/* ======================================================================== */
/* 応答結果定義(汎用)														*/
/* ======================================================================== */

#define CMD_OK					0
#define CMD_NG					1
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_NG_PRIMARY          2       /**< 優先システム不正 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_ON					1
#define CMD_OFF					0
#define CMD_TRUE				1
#define CMD_FALSE				0


/************L2 buffer usage flag*************/
#define CMD_L2_TX_SU			1		/**< buffer usage for sending S and U frame*/
#define CMD_L2_TX_I				2		/**< buffer usage for sending I frame	*/
#define CMD_L2_RX				3		/**< buffer usage for receiving frame	*/

/* ======================================================================== */
/* CPRIリンク断詳細要因定義			 										*/
/* ======================================================================== */
#define CMD_DSCLINK_INF_L1DSC		1	/**< L1断								*/
#define CMD_DSCLINK_INF_L2ABNM		2	/**< L2リンク異常						*/
#define CMD_DSCLINK_INF_L2DSC		3	/**< L2断								*/
#define CMD_DSCLINK_INF_L3DSC		4	/**< L3断								*/
#define CMD_DSCLINK_INF_L1DSCTO		5	/**< L1断「カード状態安定化時間」 TO	*/
#define CMD_DSCLINK_INF_L2ABNMTO	6	/**< L2リンク異常「カード状態安定化時間」TO*/
#define CMD_DSCLINK_INF_L2DSCTO		7	/**< L2断「カード状態安定化時間」 TO	*/
#define CMD_DSCLINK_INF_L3DSCTO		8	/**< L3断「カード状態安定化時間」 TO	*/
#define CMD_DSCLINK_INF_DIPSWCHG	9	/**< DIPSW変更							*/

/* ======================================================================== */
/* メーカMT mode定義				 										*/
/* ======================================================================== */
#define CMD_MKMTMODE_MNI			0	/**< monitor						*/
#define CMD_MKMTMODE_SIM			1	/**< simulator						*/

/* ======================================================================== */
/* CPRI simulator mode定義			 										*/
/* ======================================================================== */
#define CMD_SIMMODE_COMMON			1	/**< common mode					*/
#define CMD_SIMMODE_L3TEST			2	/**< L3 test mode					*/

/* ======================================================================== */
/* CPRI simulator link specify range		 								*/
/* ======================================================================== */
#define CMD_SIM_CPRIRANGE_SIGNLE	1	/**< signle simulator link			*/
#define CMD_SIM_CPRIRANGE_ALL		2	/**< all simulator links			*/

/****************************/
/* カード間共通関数定数定義 */
/****************************/
#define CMD_MSK_000F			0x000F		/**< 0x000F						*/
#define CMD_MSK_00F0			0x00F0		/**< 0x00F0						*/
#define CMD_MSK_00FF			0x00FF		/**< 0x00FF						*/
#define CMD_MSK_0F00			0x0F00		/**< 0x0F00						*/
#define CMD_MSK_F000			0xF000		/**< 0xF000						*/
#define CMD_MSK_FF00			0xFF00		/**< 0xFF00						*/
#define CMD_MSK_7F00			0x7F00		/**< 0xFF00						*/
#define CMD_MSK_FFF0			0xFFF0		/**< 0xFFF0						*/

#define CMD_MSK_0020			0x0020		/**< 0x0020						*/
#define CMD_MSK_0025			0x0025		/**< 0x0025						*/
#define CMD_MSK_0029			0x0029		/**< 0x0029						*/
#define CMD_MSK_0030			0x0030		/**< 0x0030						*/
#define CMD_MSK_0040			0x0040		/**< 0x0040						*/
#define CMD_MSK_0044			0x0044		/**< 0x0044						*/
#define CMD_MSK_0300			0x0300		/**< 0x0300						*/
#define CMD_MSK_00AA			0x00AA		/**< 0x00AA						*/
#define CMD_MSK_0055			0x0055		/**< 0x0055						*/
#define CMD_MSK_00A0			0x00A0		/**< 0x00A0						*/
#define CMD_MSK_0080			0x0080		/**< 0x0080						*/
#define CMD_MSK_0C00			0x0C00		/**< 0x0C00						*/
#define CMD_MSK_03FF			0x03FF		/**< 0x03FF						*/

#define CMD_MSK_FFFF0000		0xFFFF0000	/**< 0xFFFF0000					*/
#define	CMD_MSK_0000FFFF		0x0000FFFF	/**< 0x0000FFFF					*/
#define	CMD_MSK_000000FF		0x000000FF	/**< 0x000000FF					*/

/******************************/
/* EEPROM読み出し処理定数定義 */
/******************************/
#define CMD_REGMSK_EEPROMCTL	0x8000
#define CMD_REGMSK_EEPROMSEL 	0x4000
/****************************************************************************/
/*																			*/
/*							CPU関連デファイン定義							*/
/*																			*/
/****************************************************************************/
#define CMD_PDATD 			0xf0010D70
#define CMD_PDATD_D16_ON	0x00008000


/****************************************************************************/
/*																			*/
/*								ALM要因定義									*/
/*																			*/
/****************************************************************************/

#define CMD_ALMCD_SMIN			0x0000		/**< ソフト要因最小値				*/
#define CMD_ALMCD_SMAX			0x00AF		/**< ソフト要因最大値				*/
#define CMD_ALMCD_HMIN			0x00C0		/**< ハード要因最小値				*/
#define CMD_ALMCD_HMAX			0x00FF		/**< ハード要因最大値				*/

/*	システムアボート	*/
#define CMD_ALMCD_RTOS			0x0001		/**< ARTMアボート				*/
#define CMD_ALMCD_EXT_MCN		0x0002		/**< マシンチェック例外			*/
#define CMD_ALMCD_EXT_DSI		0x0003		/**< DSI例外						*/
#define CMD_ALMCD_EXT_ISI		0x0004		/**< ISI例外						*/
#define CMD_ALMCD_EXT_ALGN		0x0005		/**< アライメント例外			*/
#define CMD_ALMCD_EXT_PGM		0x0006		/**< プログラム例外				*/
#define CMD_ALMCD_EXT_TLBCMD	0x0007		/**< 命令TLBミス例外				*/
#define CMD_ALMCD_EXT_TLBDAT	0x0008		/**< データTLBミス例外		*/
#define CMD_ALMCD_EXT_TLBINS	0x0009		/**< Instruction TLBミス例外		*/
/*	共通関数要因１（システムコール系）	*/
#define CMD_ALMCD_CMNRTN		0x000A		/**< 一般システムコール関数戻り値異常	*/
#define CMD_ALMCD_TASKSTR		0x000B		/**< タスク起動異常				*/
#define CMD_ALMCD_BUFGET		0x000C		/**< バッファ取得異常			*/
#define CMD_ALMCD_BUFREL		0x000D		/**< バッファ解放異常			*/
#define CMD_ALMCD_TIMSTAT		0x000E		/**< タイマ起動異常				*/
#define CMD_ALMCD_TIMSTOP		0x000F		/**< タイマ停止異常				*/
#define CMD_ALMCD_SEMGET		0x0010		/**< セマフォ取得異常			*/
#define CMD_ALMCD_SEMREL		0x0011		/**< セマフォ返却異常			*/
#define CMD_ALMCD_CMNFUNC1		0x0012		/**< その他異常(システムコールに関するその他の異常)	*/
/*	共通関数要因２	*/
#define CMD_ALMCD_LOGICAL		0x0013		/**< 論理異常(プログラム上の論理的な異常時) */
#define CMD_ALMCD_STS			0x0014		/**< 状態異常(各種状態の異常を検出時) */
#define CMD_ALMCD_EVENT			0x0015		/**< 例外イベント受信			*/
#define CMD_ALMCD_TSKSTS		0x0015		/**< タスク状態異常				*/
#define CMD_ALMCD_CARDSTS		0x0016		/**< カード状態異常				*/
#define CMD_ALMCD_MSG			0x0017		/**< タスク間メッセージ異常		*/
#define CMD_ALMCD_USRBUF		0x0018		/**< ユーザ用バッファ領域の枯渇	*/
#define CMD_ALMCD_DATA			0x0019		/**< データ異常					*/
#define CMD_ALMCD_ROUTE			0x001A		/**< ルート異常					*/
#define CMD_ALMCD_RETURN		0x001B		/**< 関数戻り値異常				*/
#define CMD_ALMCD_CMNFUNC2		0x001C		/**< その他異常					*/

/*	プログラム要因２（準正常系動作）	*/
#define CMD_ALMCD_CARDIF		0x001D		/**< カード間IF異常				*/
#define CMD_ALMCD_CHKSUMERR		0x001E		/**< 受信データチェックサム異常	*/
#define CMD_ALMCD_PGMERR2		0x001F		/**< その他準正常異常検出		*/

/*	ソフト検出ハード要因１（メモリデバイス異常）	*/
#define CMD_ALMCD_FLASHROM		0x0020		/**< FLASH-ROM書込失敗			*/
#define CMD_ALMCD_EEPROM		0x0021		/**< EEPROMアクセス失敗			*/
#define CMD_ALMCD_SERIALBUS		0x0022		/**< シリアルバス制御失敗		*/
#define CMD_ALMCD_ETHERNET		0x0023		/**< ethernet 制御失敗			*/
#define CMD_ALMCD_FPGACNF 		0x0024		/**< FPGA CONFIG NG				*/
#define CMD_ALMCD_HARDDEVERR	0x0024		/**< その他異常検出				*/

/*	診断 	*/
#define CMD_ALMCD_FSTDIANG1			0x0025		/**< 一次診断 NG1(Boot SUM)		*/
#define CMD_ALMCD_FSTDIANG_BOOT 	0x0025		/**< 一次診断 NG1(Boot SUM)		*/
#define CMD_ALMCD_FSTDIANG_LM 		0x0026		/**< 一次診断 NG2(LM SUM)		*/
#define CMD_ALMCD_FSTDIANG_MMEN		0x0029		/**< 一次診断 NG3(Main memory)	*/
#define CMD_ALMCD_FSTDIANG_HDLC		0x0029		/**< 一次診断 NG4(HDLC RW )		*/
#define CMD_ALMCD_FSTDIANG_CFPGA	0x0029		/**< 一次診断 NG5(CPRI-FPGA SUM)*/
#define CMD_ALMCD_FSTDIANG_AFPGA	0x0029		/**< 一次診断 NG5(APD-FPGA SUM)	*/
#define CMD_ALMCD_FSTDIANG_CFPGARW 	0x0029		/**< 一次診断 NG5(CPRI-FPGA RW)	*/
#define CMD_ALMCD_FSTDIANG_AFPGARW 	0x0029		/**< 一次診断 NG5(APD-FPGA RW)	*/
#define CMD_ALMCD_FSTDIANG_FPGACNF 	0x0029		/**< 一次診断 NG5(FPGA CONFIG INCOMPLETE )	*/

#define CMD_ALMCD_WDTTO				0x002A		/**< WDTTO ALM (Firmware) */
#define CMD_ALMCD_CHKSTP			0x002B		/**< Check STOP ALM */

#define CMD_ALMCD_REMOTEALM			0x00AF		/**< remote ALM from AP */
#define CMD_ALMCD_EXTPLL			0x00B0		/**< EXT PLL ALM		 */
#define CMD_ALMCD_EXT0GAIN			0x00B1		/**< EXT＃0-GAIN ALM 	*/
#define CMD_ALMCD_EXT1GAIN			0x00B2		/**< EXT＃1-GAIN ALM 	*/
#define CMD_ALMCD_EXT0LNA			0x00B3		/**< EXT＃0-LNA ALM 	*/
#define CMD_ALMCD_EXT1LNA			0x00B4		/**< EXT＃1-LNA ALM 	*/

/*	ハードALM	*/
/*								0xC0 ～	0xFF								*/

/* PLL ALM */
#define CMD_ALMCD_PLLUNLOCK		0x00C0		/**< PLL UNLOCK	*/
#define CMD_ALMCD_CPRIUNLOCK 	0x00C1		/**< CPRI_FPGA-PLL-UNLOCK */
#define CMD_ALMCD_APD0UNLOCK	0x00C2		/**< APD#0_FPGA-PLL-UNLOCK*/	
#define CMD_ALMCD_APD1UNLOCK	0x00C3		/**< APD#1_FPGA-PLL-UNLOCK*/	

#define CMD_ALMBIT_PLLUNLOCK	0x00000002		/**< PLL(AD9523)ANLOCK ALM	*/
#define CMD_ALMBIT_CPRIUNLOCK 	0x00000001		/**< CPRI PLL UNLOCK ALM */

/* CPRI ALM */
#define CMD_ALMCD_WDTCPU		0x00D0		/**< WDT TO (CPU detected) */
#define CMD_ALMCD_SYSERR		0x00D1		/**< SYS-ERR ALM */
#define CMD_ALMCD_STXFAULT		0x00D2		/**< STXFAULT ALM */
#define CMD_ALMCD_CPUPRTY		0x00D3		/**< CPUPRTY ALM */

/* APD_COM ALM */
#define CMD_ALMCD_DLBSY0		0x00D8		/**< DLBSY ALM#0 */
#define CMD_ALMCD_STARTUP0 		0x00D9		/**< STARTUP ALM#0 */
#define CMD_ALMCD_PS0			0x00DA		/**< PS ALM#0 */
#define CMD_ALMCD_DLBSY1		0x00DB		/**< DLBSY ALM#1 */
#define CMD_ALMCD_STARTUP1		0x00DC		/**< STARTUP ALM#1 */
#define CMD_ALMCD_PS1			0x00DD		/**< PS ALM#1 */

#define CMD_ALMBIT_PS0 		0x00000100		/**< PS ALM#0*/	
#define CMD_ALMBIT_STARTUP0	0x00000080		/**< STARTUP ALM#0 */	
#define CMD_ALMBIT_PS1		0x00000020		/**< PS ALM#1 */	
#define CMD_ALMBIT_STARTUP1	0x00000010		/**< STARTUP ALM#1*/	

/* APD0 ALM */
#define CMD_ALMCD_WDT0			0x00E0		/**< WDT ALM#0 */
#define CMD_ALMCD_SPI0			0x00E1		/**< SPI ALM#0 */
#define CMD_ALMCD_FWPLL0		0x00E2		/**< FW PLL ALM#0 */
#define CMD_ALMCD_FBPLL0		0x00E3		/**< FB PLL ALM#0 */
#define CMD_ALMCD_RXPLL0		0x00E4		/**< RX PLL ALM#0 */
#define CMD_ALMCD_TEMP0			0x00E8		/**< TEMP ALM#0 */
#define CMD_ALMCD_IM0			0x00E9		/**< IM ALM#0 */
#define CMD_ALMCD_GAIN0			0x00EA		/**< GAIN ALM#0 */
#define CMD_ALMCD_TOVER0		0x00EB		/**< TOVER ALM#0 */
#define CMD_ALMCD_RXGAIN0		0x00EC		/**< RX GAIN ALM#0 */
#define CMD_ALMCD_FILSW0		0x00EE		/**< FILSW STATUS ALM#0 */

#define CMD_ALMBIT_FWPLL0		0x00010000		/**< FW PLL ALM#0 */
#define CMD_ALMBIT_RXPLL0		0x00020000		/**< RX PLL ALM#0 */
#define CMD_ALMBIT_TEMP0		0x00040000		/**< TEMP ALM#0 */
#define CMD_ALMBIT_IM0			0x00080000		/**< IM ALM#0 */
#define CMD_ALMBIT_GAIN0		0x00100000		/**< GAIN ALM#0 */
#define CMD_ALMBIT_TOVER0		0x00200000		/**< TOVER ALM#0 */
#define CMD_ALMBIT_RXGAIN0		0x00400000	/**< RX GAIN ALM#0 */
#define CMD_ALMBIT_FILSW0		0x00800000	/**< FILSW STATUS  ALM#0 */

/* APD1 ALM */
#define CMD_ALMCD_WDT1			0x00F0		/**< WDT ALM#1 */
#define CMD_ALMCD_SPI1			0x00F1		/**< SPI ALM#1 */
#define CMD_ALMCD_FWPLL1		0x00F2		/**< FW PLL ALM#1 */
#define CMD_ALMCD_FBPLL1		0x00F3		/**< FB PLL ALM#1 */
#define CMD_ALMCD_RXPLL1		0x00F4		/**< RX PLL ALM#1 */
#define CMD_ALMCD_TEMP1			0x00F8		/**< TEMP ALM#1 */
#define CMD_ALMCD_IM1			0x00F9		/**< IM ALM#1 */
#define CMD_ALMCD_GAIN1			0x00FA		/**< GAIN ALM#1 */
#define CMD_ALMCD_TOVER1		0x00FB		/**< TOVER ALM#1 */
#define CMD_ALMCD_RXGAIN1		0x00FC		/**< RX GAIN ALM#1 */
#define CMD_ALMCD_FILSW1		0x00FE		/**< FILSW STATUS ALM#0 */

#define CMD_ALMBIT_FWPLL1		0x01000000		/**< FW PLL ALM#1 */
#define CMD_ALMBIT_RXPLL1		0x02000000		/**< RX PLL ALM#1 */
#define CMD_ALMBIT_TEMP1		0x04000000		/**< TEMP ALM#1 */
#define CMD_ALMBIT_IM1			0x08000000		/**< IM ALM#1 */
#define CMD_ALMBIT_GAIN1		0x10000000		/**< GAIN ALM#1 */
#define CMD_ALMBIT_TOVER1		0x20000000		/**< TOVER ALM#1 */
#define CMD_ALMBIT_RXGAIN1		0x40000000		/**< RX GAIN ALM#1 */
#define CMD_ALMBIT_FILSW1		0x80000000	/**< FILSW STATUS  ALM#1 */


/* ======================================================================== */
/* 小型RRE(SLC)のALM要因定義												*/
/* ======================================================================== */
#define CMD_ALMCD_SLC_NUMMAX	        92		/**< SLC-RRE ALM要因数 (値の重複を含む)	*/

/*	システムアボート	*/
#define CMD_ALMCD_SLC_RTOS			0x0031		/**< ARTMアボート				*/
#define CMD_ALMCD_SLC_EXT_MCN		0x0032		/**< マシンチェック例外			*/
#define CMD_ALMCD_SLC_EXT_DSI		0x0033		/**< DSI例外					*/
#define CMD_ALMCD_SLC_EXT_ISI		0x0034		/**< ISI例外					*/
#define CMD_ALMCD_SLC_EXT_ALGN		0x0035		/**< アライメント例外			*/
#define CMD_ALMCD_SLC_EXT_PGM		0x0036		/**< プログラム例外				*/
#define CMD_ALMCD_SLC_EXT_TLBCMD	0x0037		/**< 命令TLBミス例外			*/
#define CMD_ALMCD_SLC_EXT_TLBDAT	0x0038		/**< データTLBミス例外			*/
#define CMD_ALMCD_SLC_EXT_TLBINS	0x0039		/**< Instruction TLBミス例外	*/
/*	共通関数要因１（システムコール系）	*/
#define CMD_ALMCD_SLC_CMNRTN		0x003A		/**< 一般システムコール関数戻り値異常	*/
#define CMD_ALMCD_SLC_TASKSTR		0x003B		/**< タスク起動異常				*/
#define CMD_ALMCD_SLC_BUFGET		0x003C		/**< バッファ取得異常			*/
#define CMD_ALMCD_SLC_BUFREL		0x003D		/**< バッファ解放異常			*/
#define CMD_ALMCD_SLC_TIMSTAT		0x003E		/**< タイマ起動異常				*/
#define CMD_ALMCD_SLC_TIMSTOP		0x003F		/**< タイマ停止異常				*/
#define CMD_ALMCD_SLC_SEMGET		0x0040		/**< セマフォ取得異常			*/
#define CMD_ALMCD_SLC_SEMREL		0x0041		/**< セマフォ返却異常			*/
#define CMD_ALMCD_SLC_CMNFUNC1		0x0042		/**< その他異常(システムコールに関するその他の異常)	*/
/*	共通関数要因２	*/
#define CMD_ALMCD_SLC_LOGICAL		0x0043		/**< 論理異常(プログラム上の論理的な異常時) */
#define CMD_ALMCD_SLC_STS			0x0044		/**< 状態異常(各種状態の異常を検出時) */
#define CMD_ALMCD_SLC_EVENT			0x0045		/**< 例外イベント受信			*/
#define CMD_ALMCD_SLC_TSKSTS		0x0045		/**< タスク状態異常				*/
#define CMD_ALMCD_SLC_CARDSTS		0x0046		/**< カード状態異常				*/
#define CMD_ALMCD_SLC_MSG			0x0047		/**< タスク間メッセージ異常		*/
#define CMD_ALMCD_SLC_USRBUF		0x0048		/**< ユーザ用バッファ領域の枯渇	*/
#define CMD_ALMCD_SLC_DATA			0x0049		/**< データ異常					*/
#define CMD_ALMCD_SLC_ROUTE			0x004A		/**< ルート異常					*/
#define CMD_ALMCD_SLC_RETURN		0x004B		/**< 関数戻り値異常				*/
#define CMD_ALMCD_SLC_CMNFUNC2		0x004C		/**< その他異常					*/

/*	プログラム要因２（準正常系動作）	*/
#define CMD_ALMCD_SLC_CARDIF		0x004D		/**< カード間IF異常				*/
#define CMD_ALMCD_SLC_CHKSUMERR		0x004E		/**< 受信データチェックサム異常	*/
#define CMD_ALMCD_SLC_PGMERR2		0x004F		/**< その他準正常異常検出		*/

/*	ソフト検出ハード要因１（メモリデバイス異常）	*/
#define CMD_ALMCD_SLC_FLASHROM		0x0050		/**< FLASH-ROM書込失敗			*/
#define CMD_ALMCD_SLC_EEPROM		0x0051		/**< EEPROMアクセス失敗			*/
#define CMD_ALMCD_SLC_SERIALBUS		0x0052		/**< シリアルバス制御失敗		*/
#define CMD_ALMCD_SLC_ETHERNET		0x0053		/**< ethernet 制御失敗			*/
#define CMD_ALMCD_SLC_FPGACNF 		0x0054		/**< FPGA CONFIG NG				*/
#define CMD_ALMCD_SLC_HARDDEVERR	0x0054		/**< その他異常検出				*/

/*	診断 	*/
#define CMD_ALMCD_SLC_FSTDIANG1			0x0055		/**< 一次診断 NG1(Boot SUM)		*/
#define CMD_ALMCD_SLC_FSTDIANG_BOOT 	0x0055		/**< 一次診断 NG1(Boot SUM)		*/
#define CMD_ALMCD_SLC_FSTDIANG_LM 		0x0056		/**< 一次診断 NG2(LM SUM)		*/
#define CMD_ALMCD_SLC_FSTDIANG_MMEN		0x0057		/**< 一次診断 NG3(Main memory)	*/
#define CMD_ALMCD_SLC_FSTDIANG_HDLC		0x0058		/**< 一次診断 NG4(HDLC RW )		*/
#define CMD_ALMCD_SLC_FSTDIANG_CFPGA	0x0059		/**< 一次診断 NG5(CPRI-FPGA SUM)	*/
#define CMD_ALMCD_SLC_FSTDIANG_AFPGA	0x0059		/**< 一次診断 NG5(APD-FPGA SUM)	*/
#define CMD_ALMCD_SLC_FSTDIANG_CFPGARW 	0x0059		/**< 一次診断 NG5(CPRI-FPGA RW)	*/
#define CMD_ALMCD_SLC_FSTDIANG_AFPGARW 	0x0059		/**< 一次診断 NG5(APD-FPGA RW)	*/
#define CMD_ALMCD_SLC_FSTDIANG_FPGACNF 	0x0059		/**< 一次診断 NG5(FPGA CONFIG INCOMPLETE )	*/

#define CMD_ALMCD_SLC_WDTTO				0x005A		/**< WDTTO ALM (Firmware)	*/
#define CMD_ALMCD_SLC_CHKSTP			0x005B		/**< Check STOP ALM		*/

#define CMD_ALMCD_SLC_REMOTEALM			0x005F		/**< remote ALM from AP */
#define CMD_ALMCD_SLC_EXTPLL			0x0060		/**< EXT PLL ALM		*/
#define CMD_ALMCD_SLC_EXT0GAIN			0x0061		/**< EXT＃0-GAIN ALM 	*/
#define CMD_ALMCD_SLC_EXT1GAIN			0x0062		/**< EXT＃1-GAIN ALM 	*/
#define CMD_ALMCD_SLC_EXT0LNA			0x0063		/**< EXT＃0-LNA ALM 	*/
#define CMD_ALMCD_SLC_EXT1LNA			0x0064		/**< EXT＃1-LNA ALM 	*/

/*	ハードALM	*/

/* PLL ALM */
#define CMD_ALMCD_SLC_PLLUNLOCK			0x0068		/**< PLL UNLOCK				*/
#define CMD_ALMCD_SLC_CPRIUNLOCK 		0x0069		/**< CPRI_FPGA-PLL-UNLOCK	*/
#define CMD_ALMCD_SLC_APD0UNLOCK		0x006A		/**< APD#0_FPGA-PLL-UNLOCK	*/
#define CMD_ALMCD_SLC_APD1UNLOCK		0x006B		/**< APD#1_FPGA-PLL-UNLOCK	*/

/* CPRI ALM */
#define CMD_ALMCD_SLC_WDTCPU			0x0070		/**< WDT TO (CPU detected)	*/
#define CMD_ALMCD_SLC_SYSERR			0x0071		/**< SYS-ERR ALM			*/
#define CMD_ALMCD_SLC_STXFAULT			0x0072		/**< STXFAULT ALM			*/
#define CMD_ALMCD_SLC_CPUPRTY			0x0073		/**< CPUPRTY ALM			*/

/* APD_COM ALM */
#define CMD_ALMCD_SLC_DLBSY0			0x0078		/**< DLBSY ALM#0			*/
#define CMD_ALMCD_SLC_STARTUP0 			0x0079		/**< STARTUP ALM#0			*/
#define CMD_ALMCD_SLC_PS0				0x007A		/**< PS ALM#0				*/
#define CMD_ALMCD_SLC_DLBSY1			0x007B		/**< DLBSY ALM#1			*/
#define CMD_ALMCD_SLC_STARTUP1			0x007C		/**< STARTUP ALM#1			*/
#define CMD_ALMCD_SLC_PS1				0x007D		/**< PS ALM#1				*/

/* APD0 ALM */
#define CMD_ALMCD_SLC_WDT0			0x0080		/**< WDT ALM#0				*/
#define CMD_ALMCD_SLC_SPI0			0x0081		/**< SPI ALM#0				*/
#define CMD_ALMCD_SLC_FWPLL0		0x0082		/**< FW PLL ALM#0			*/
#define CMD_ALMCD_SLC_FBPLL0		0x0083		/**< FB PLL ALM#0			*/
#define CMD_ALMCD_SLC_RXPLL0		0x0084		/**< RX PLL ALM#0			*/
#define CMD_ALMCD_SLC_TEMP0			0x0088		/**< TEMP ALM#0				*/
#define CMD_ALMCD_SLC_IM0			0x0089		/**< IM ALM#0				*/
#define CMD_ALMCD_SLC_GAIN0			0x008A		/**< GAIN ALM#0				*/
#define CMD_ALMCD_SLC_TOVER0		0x008B		/**< TOVER ALM#0			*/
#define CMD_ALMCD_SLC_RXGAIN0		0x008C		/**< RX GAIN ALM#0			*/
#define CMD_ALMCD_SLC_FILSW0		0x008E		/**< RX GAIN ALM#0			*/

/* APD1 ALM */
#define CMD_ALMCD_SLC_WDT1			0x0090		/**< WDT ALM#1				*/
#define CMD_ALMCD_SLC_SPI1			0x0091		/**< SPI ALM#1				*/
#define CMD_ALMCD_SLC_FWPLL1		0x0092		/**< FW PLL ALM#1			*/
#define CMD_ALMCD_SLC_FBPLL1		0x0093		/**< FB PLL ALM#1			*/
#define CMD_ALMCD_SLC_RXPLL1		0x0094		/**< RX PLL ALM#1			*/
#define CMD_ALMCD_SLC_TEMP1			0x0098		/**< TEMP ALM#1				*/
#define CMD_ALMCD_SLC_IM1			0x0099		/**< IM ALM#1				*/
#define CMD_ALMCD_SLC_GAIN1			0x009A		/**< GAIN ALM#1				*/
#define CMD_ALMCD_SLC_TOVER1		0x009B		/**< TOVER ALM#1			*/
#define CMD_ALMCD_SLC_RXGAIN1		0x009C		/**< RX GAIN ALM#1			*/
#define CMD_ALMCD_SLC_FILSW1		0x009E		/**< FILSW STATUS ALM#1			*/
/****************************************************************************/
/*																			*/
/*								ERR要因定義									*/
/*																			*/
/****************************************************************************/

//TD-SRE
#define CMD_ERRCD_BR0_SLEEP		0x000C		/**< SLP0 ERR bit in SV A8/A9		*/
#define CMD_ERRCD_BR1_SLEEP		0x000D		/**< SLP1 ERR bit in SV A8/A9		*/
#define CMD_ERRCD_BR2_SLEEP		0x000E		/**< SLP2 ERR bit in SV A8/A9		*/
#define CMD_ERRCD_BR3_SLEEP		0x000F		/**< SLP3 ERR bit in SV A8/A9		*/
#define CMD_ERRCD_L1_ERR		0x0001
#define CMD_ERRCD_L2_ERR		0x0002
#define CMD_ERRCD_L3_ERR		0x0003
#define CMD_ERRCD_AMP_POWOFF	0x0008		/**< AMP POWER OFF */
#define CMD_ERRCD_DL_FILINFO	0x0004		/**< FILE info report NG */
#define CMD_ERRCD_DL_FILDAT		0x0005		/**< FILE data sending NG */
#define CMD_ERRCD_REMOTE_ERR	0x0020		/**< REMOTE ERROR */
//TD-RRE
#define CMD_ERRCD_SFPNOSUPTERR		0x0009		/**< SFP no support error */
#define CMD_ERRCD_AISG_ERR			0x0010		/**< RET未接続 */
#define CMD_ERRCD_CPS_L1_ERR		0x0015		/**< CPS CPRI L1 Disconnect	*/
#define CMD_ERRCD_CPS_L2_ERR		0x0016		/**< CPS CPRI L2 Disconnect	*/
#define CMD_ERRCD_CPS_L3_ERR		0x0017		/**< CPS CPRI L3 Disconnect	*/
#define CMD_ERRCD_CAS_CABL_LEN		0x001A		/**< Cascade cable length error	*/
#define CMD_ERRCD_CPS_SFP_UNMNT		0x001D		/**< CPS SFP unmounted		*/
#define CMD_ERRCD_SFPNOSUPTERR_CHLD	0x0029		/**< SFP unmounted		*/
#define CMD_ERRCD_AISG_POW			0x002B		/**< AISG POWER ERR */
#define CMD_ERRCD_FAN_POW			0x002C		/**< FAB POWER ERR */
#define CMD_ERRCD_FAN_POW_CHLD		0x002D		/**< FAB POWER ERR(Child) 	*/
/* @} */

#define CMD_LINK_NUM_M				1	/**< Amount of CPRI links in each TRA card*/

/** @name TRX set/release related */
/* @{ */

/* ======================================================================== */
/* TRA設定/解放通知															*/
/* ======================================================================== */
#define CMD_TRA_STACHG_ON		1			/**< TRA状態変化あり				*/
#define CMD_TRA_STACHG_OFF		0			/**< TRA状態変化なし				*/
/* @} */


/** @name ALM information (RE card state report related) */
/* @{ */
/* ======================================================================== */
/* REカード状態報告関連														*/
/* ======================================================================== */
//3.5G-SRE
#define CMD_MAX_ALM_NUM			255			/**< 報告する最大ALM数			*/
#define CMD_MAX_ERR_NUM			255			/**< 報告する最大ERR数			*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
//4.9/9.8G
#define CMD_MAX_ALM_NUM_HIG_BITRAT	5107
#define CMD_MAX_ERR_NUM_HIG_BITRAT	5107
#define CMD_MAX_SIZ_STA_REP			10250
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* ======================================================================== */
/* 無効値																	*/
/* ======================================================================== */
#define CMD_IVL_VAL				65535		/**< 無効値(65535)				*/

/* @} */

/** @name Slot number */
/* @{ */

/* ======================================================================== */
/* スロット番号関連															*/
/* ======================================================================== */
/* AMP type (TBD)				*/
#define CMD_AMP_MAX_SLOTNUM		6			/**< AMP時の最大スロット数		*/
#define CMD_AMP_MIN_SLOTNO		1			/**< AMP時の最小スロット番号	*/
#define CMD_AMP_MAX_SLOTNO		6			/**< AMP時の最大スロット番号	*/

/* OF-TRX	type					*/
#define CMD_OF_MAX_SLOTNUM		1			/**< OF-TRX時の最大スロット数	*/
#define CMD_OF_MIN_SLOTNO		1			/**< OF-TRX時の最小スロット番号	*/
#define CMD_OF_MAX_SLOTNO		1			/**< OF-TRX時の最大スロット番号	*/

/* @} */

/** @name Max/Min carrier number */
/* @{ */

/* ======================================================================== */
/* キャリア番号関連															*/
/* ======================================================================== */
#define CMD_MIN_CARNUM			1			/**< Min carrier number				*/
/*TDD SRE  TRX setting S. */
#define CMD_MAX_CARNUM			2			/**< Max carrier number				*/
/*TDD SRE  TRX setting E. */
#define CMD_MIN_CARNUM_S3G		1			/**< Min carrier number for S3G		*/
//TD-SRE update
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_MAX_CARNUM_3G		4			/**< Max carrier number for 3G		*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_MAX_CARNUM_S3G		2			/**< Max carrier number for S3G		*/
#define CMD_MAX_CARDNUM			1			/**< Max card number under 1 CPRI link	*/

/* @} */

/****************************************************************************/
/*	- No.8_2 -			other common definition								*/
/****************************************************************************/

/** @name Max transition size */
/* @{ */

#define CMD_MAX_TRNS_SIZ		10240		/**< 最大送信サイズ				*/

/* @} */

/** @name Start Boot face number in EEPROM */
/* @{ */

/* ======================================================================== */
/* EEPROM関連(起動面)														*/
/* ======================================================================== */
#define CMD_BOOT0				0			/**< 0面起動						*/
#define CMD_BOOT1				1			/**< 1面起動						*/
/* @} */

/** @name LRE device type in EEPROM */
/* @{ */

/* ======================================================================== */
/* EEPROM関連(LRE装置タイプ)												*/
/* ======================================================================== */
#define CMD_EEP_LRETYP_DC		1			/**< 2GLR2AD(DC装置)			*/
#define CMD_EEP_LRETYP_AC		2			/**< 2GLR2AA(AC装置)			*/
#define CMD_EEP_LRETYP_CRD		3			/**< 2GCLR2A(カード)			*/
#define CMD_EEP_LRETYP_DC_15G17G	4			/**< 15G-17G-LRE(DC装置)		*/
#define CMD_EEP_LRETYP_AC_15G17G	5			/**< 15G-17G-LRE(AC装置)		*/
/* @} */

/** @name TCP number for CPRI-MT and RE-MT */
/* @{ */

/* ======================================================================== */
/* TCP number for CPRI-MT and RE-MT											*/
/* ======================================================================== */
#define CMD_MX_TCPNO			3		/**< Max TCP link number (CPRI-MT/RE-MT) */

/* ======================================================================== */
/*  Definition for FTP part									*/
/* ======================================================================== */
#define   CMD_FLD_FTP					0		/**< Protocol type in L3 msg of FLD data send/receive request*/
#define   CMD_FLD_TCP					1		/**< Protocol type in L3 msg of FLD data send/receive request*/

#define   CMD_FTP_CMD                   0		/**< Data type in L3 msg of FLD data send/receive request*/
#define   CMD_FTP_FIL					1		/**< Data type in L3 msg of FLD data send/receive request*/

#define   CMD_IPPORT_FTPCMD			21				/**< FTP command  transfer port*/
#define   CMD_IPPORT_FTPFIL			20				/**< FTP file transfer port*/
#define   CMD_IPPORT_UDPCOMM		1234			/**< UDP port used for internal communcation*/
#define   CMD_IPPORT_UDPSRV			5678			/**< UDP port used for internal communcation*/
#define   CMD_IPADDR_SELF			0x7f000001		/**< Self IP address*/

/* @} */

/** @name connected system number for shared-RE */
/* @{ */

/* ======================================================================== */
/* Connected system number for shared-RE									*/
/* ======================================================================== */

#define CMD_MAX_SYSNUM			2	/**< Max connected system number for shared-RE */
#define CMD_SYS_3G				0	/**< connected system is 3G */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_SYS_LTE				1	/**< connected system is LTE */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define CMD_SYS_S3G				1	/**< connected system is S3G */
#define CMD_SYS_BOTH_3G_S3G		2	/**< connected system is both 3G and S3G */

#define CMD_SYS_MSK_CPRID		0xFFF0	/**< Mask the system information in CPRI Signal ID */
#define CMD_SYS_CHK_CPRID		0x0001	/**< Check the system infromn in CPRI signal ID */


/* ======================================================================== */
/* Connected operation mode							*/
/* ======================================================================== */

#define CMD_MAX_OPNUM			4	/**< Max connected operation mode */
#define CMD_OP_UNK				0	/**< connected operation is unknow */
#define CMD_OP_LTE				1	/**< connected operation is LTE */
#define CMD_OP_NR				2	/**< connected operation is NR */
#define CMD_OP_LTE_NR			3	/**< connected operation is LTE+NR */

/* @} */

/** @name 接続構成種別 for shared-RE */
/* @{ */

/* ======================================================================== */
/* 接続構成種別 for shared-RE									*/
/* ======================================================================== */

#define CMD_CONN_3G_SINGLE		0	/**< 接続構成 is 3G単独 */
#define CMD_CONN_S3G_SINGLE		1	/**< 接続構成 is S3G単独 */
#define CMD_CONN_CPRI_SHARED	2	/**< 接続構成 is CPRI共用 */
#define CMD_CONN_RF_SHARED		3	/**< 接続構成 is RF共用 */
/* @} */



/** @name Max system parameter length */
/* @{ */
/* Max system parameter length */
#define CMD_SYSPRAD_MAX			16		/* max system parameter D length			*/
#define CMD_SYSPRARCV_MAX		32		/* Received max system parameter length(SYSPAR-D +SYSPAR-E+ SYSPAR-F) */


/* @} */


/****************************************************************************/
/*	- No.9 -																*/
/*					TRAカード内共通テーブル関連定義						*/
/*																			*/
/****************************************************************************/

/* ======================================================================== */
/* RE装置構成情報テーブル													*/
/* ======================================================================== */

/** @name RE device composition definition (cmw_re_dev_tbl)*/
/* @{ */

#define CMD_REKIND_AMP				0x0000		/**< RE type is BRE(AMP)		*/
#define CMD_REKIND_OFTRX			0x0001		/**< RE type is RRE/LRE(OFTRX)	*/

#define CMD_DIV_ON					0x0001		/**< 装置号番がDIV有り			*/
#define CMD_DIV_OFF					0x0000		/**< 装置号番がDIV無し			*/

#define CMD_PTYP_DC					0x0000		/**< DC電源						*/
#define CMD_PTYP_100				0x0001		/**< AC100V						*/
#define CMD_PTYP_200				0x0002		/**< AC200V						*/
#define CMD_PTYP_COM				0x0003		/**< AC100/200V					*/

#define CMD_PWR_CLASS_10			10			/**< power class 10W			*/
#define CMD_PWR_CLASS_5				5			/**< power class 5W				*/
#define CMD_PWR_CLASS_125			125			/**< power class 0.125MW		*/
#define CMD_PWR_CLASS_6300			0x189C		/**< power class 6300MW			*/
#define CMD_PWR_CLASS_667			0x029B		/**< power class 667MW			*/

#define CMD_DEVKND_RRE				0x0001	/**< device kind for RRE 				*/
#define CMD_DEVKND_BRECPRI			0x0002	/**< device kind for BRE-CPRI			*/
#define CMD_DEVKND_BRERF			0x0003	/**< device kind for BRE-RF				*/
#define CMD_DEVKND_LRE2T2R			0x0004	/**< device kind for LRE-2T2R			*/
#define CMD_DEVKND_LRE1T2R			0x0005	/**< device kind for LRE-1T1R			*/
#define CMD_DEVKND_LRE2T2R_CRD		0xFFFE	/**< device kind for LRE-2T2R(card type)	*/
#define CMD_DEVKND_LRE1T2R_CRD		0xFFFF	/**< device kind for LRE-1T1R(card type)	*/
#define CMD_DEVKND_RRE_800M			0x0006	/**< device kind for 800M-RRE			*/
#define CMD_DEVKND_RRE_2GCD			0x0007	/**< device kind for 2G CD-RRE			*/
#define CMD_DEVKND_SRE_15G			0x0008	/**< device kind for SRE 15G			*/
#define CMD_DEVKND_RRE_ECO			0x0008	/**< device kind for 800M-RRE			*/
#define CMD_DEVKND_RRE_15G			0x0009	/**< device kind for 1.5G-RRE			*/
#define CMD_DEVKND_RRE_800M_SLC		0x000A	/**< device kind for 800M-RRE(SLC)			*/
#define CMD_DEVKND_RRE_800M_SLCHIGH	0x000B	/**< device kind for 800M-RRE(SLC 高出力)	*/
#define CMD_DEVKND_RRE_2G_SLC		0x000C	/**< device kind for 2G-RRE(SLC)			*/
#define CMD_DEVKND_RRE_2G_SLCHIGH	0x000D	/**< device kind for 2G-RRE(SLC 高出力)		*/
#define CMD_DEVKND_TRA_CPRI_CD		0x000E	/**< device kind for TRA CPRIコストダウン	*/
#define CMD_DEVKND_RRE_15G_VA		0x0100	/**< device kind for 1.5G (SLC)	*/
#define CMD_DEVKND_RRE_17G			0x0101	/**< device kind for 1.7G-RRE	*/
#define CMD_DEVKND_RRE_700M			0x0102	/**< device kind for 700M-RRE	*/
#define CMD_DEVKND_XRE_RSV1			0x0103	/**< device kind for Reserve No1	*/
#define CMD_DEVKND_XRE_RSV2			0x0104	/**< device kind for Reserve No2	*/
#define CMD_DEVKND_XRE_RSV3			0x0105	/**< device kind for Reserve No3	*/
#define CMD_DEVKND_LRE_15G			0x0106	/**< device kind for 15G LRE	*/
#define CMD_DEVKND_LRE_17G			0x0107	/**< device kind for 17G LRE	*/
#define CMD_DEVKND_RRE_15G_TRI		0x0108	/**< device kind for 15G RRE	*/
#define CMD_DEVKND_RRE_17G_TRI		0x0109	/**< device kind for 17G RRE	*/
#define CMD_DEVKND_RRE_700M_TRI		0x010A	/**< device kind for 700M-RRE	*/
#define CMD_DEVKND_RRE_15G_SRE		0x010B	/**< device kind for 15G-RRE	*/
#define CMD_DEVKND_RRE_35G_SRE		0x010C	/**< device kind for 35G-SRE	*/

#define CMD_DEVKIND_OFFSET_1CHP 	CMD_DEVKND_RRE_15G_VA  /**< offset for 1chp 	*/
#define CMD_DEVKIND_OFFSET_ZYNQ 	CMD_DEVKND_RRE_15G_TRI  /**< offset for zynq 	*/

#define CMD_DEVKND_MAX				(CMD_DEVKND_LRE_17G+1)	/**< device kind for MAX 	*/

/* 対応周波数帯 */

#define CMD_CRP_FRQ_2G_S3G			1		/**< 対応周波数帯 2G(S3G報告値)		*/
#define CMD_CRP_FRQ_15G_S3G			2		/**< 対応周波数帯 1.5G(S3G報告値)	*/
#define CMD_CRP_FRQ_800M_S3G		4		/**< 対応周波数帯 800M(S3G報告値)	*/

#define CMD_CRP_FRQ_700M_S3G		8		/**< 対応周波数帯 700(S3G報告値)	*/
#define CMD_CRP_FRQ_17G_S3G			16		/**< 対応周波数帯 1.7G(S3G報告値)	*/
#define CMD_CRP_FRQ_35G_S3G		    64		/**< 対応周波数帯 700(S3G報告値)	*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CRP_FRQ_17G_L_S3G		CMD_FRQBAND_17L_LTE		/**< 対応周波数帯 1.7G(S3G報告値)	0x0020 (LowBand帯)*/
#define CMD_CRP_FRQ_34G_S3G			CMD_FRQBAND_34_LTE		/**< 対応周波数帯 3.4G(S3G報告値)	0x0080 */
#define CMD_CRP_FRQ_ALL_S3G			(CMD_FRQBAND_2_LTE | \
									 CMD_FRQBAND_15_LTE | \
									 CMD_FRQBAND_800_LTE | \
									 CMD_FRQBAND_700_LTE | \
									 CMD_FRQBAND_17_LTE | \
									 CMD_FRQBAND_17L_LTE | \
									 CMD_FRQBAND_35_LTE | \
									 CMD_FRQBAND_34_LTE)		/**< 全対応周波数帯(サポート判定用)	0x00FF */

#define CMD_CRP_FRQ_2G_3G			0		/**< 対応周波数帯 2G(3G報告値)		*/
#define CMD_CRP_FRQ_15G_3G			7		/**< 対応周波数帯 1.5G(3G報告値)	*/
#define CMD_CRP_FRQ_800M_3G			1		/**< 対応周波数帯 800M(3G報告値)	*/
#define CMD_CRP_FRQ_17G_3G			2		/**< 対応周波数帯 1.7G(3G報告値)	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_BANDW_MAX_5M			1		/**< Max bandwidth is 5M */
#define CMD_BANDW_MAX_10M			2		/**< Max bandwidth is 10M */
#define CMD_BANDW_MAX_15M			3		/**< Max bandwidth is 15M */
#define CMD_BANDW_MAX_20M			4		/**< Max bandwidth is 20M */

#define CMD_SND_ANTE_MAX_1			1		/**< Max sending antenna number is 1 */
#define CMD_SND_ANTE_MAX_2			2		/**< Max sending antenna number is 2 */


/** @name card control item definition (cmw_cardcnt_tbl)*/
/* @{ */

/* ======================================================================== */
/*  カード制御状態テーブル( rew_cardcnt_tbl )								*/
/* ======================================================================== */

/****************************************************************************/
/* card control item														*/
/****************************************************************************/
/* card control item wich is defined in card status report response */

//TDD-SRE
#define CMD_LOCAL_BLKOFF		0x1000		/**< local block cancel			*/
#define CMD_LOCAL_BLK			0x2000		/**< local block				*/
#define CMD_REMOTE_ALM			0x0002		/**< remote ALM					*/
#define CMD_REMOTE_CMPBLK		0x0004		/**< remote force block			*/
#define CMD_REMOTE_CHK			0x0001		/**< remote Check				*/
#define CMD_REMOTE_BLKOFF		0x0010		/**< remote block cancel		*/
#define CMD_REMOTE_BLK			0x0020		/**< remote block				*/
//TDD-SRE

/* card control item which is defined internally */
#define CMD_LOCAL_ERR			0x0080		/**< local ERR					*/
#define CMD_LOCAL_ERROFF		0x0040		/**< local ERR OFF				*/

/* 制御項目以外マスク */
/* ローカル制御時に報告不要な項目をマスクする */
#define CMD_NOCARDCNTITEM_MASK	0x3000		/**< 制御項目以外マスク			*/

/* リモート/ローカル制御 */
#define CMD_RMT_CNT				0			/**< リモート制御					*/
#define CMD_LCL_CNT				1			/**< ローカル制御					*/

/* @} */


/* @} */

/** @name Alarm factor number definition */
/* @{ */

#define CMD_ALMFACTOR_NUM			6			/* アラーム要因数		*/

/* @} */

/** @name BR number definition */
/* @{ */


/* ======================================================================== */
/* branch bit 														*/
/* ======================================================================== */
//TD-SRE update
#define CMD_BR0_BIT					0x0001		/**< BR0							*/
#define CMD_BR1_BIT					0x0002		/**< BR1							*/
#define CMD_BR2_BIT					0x0004		/**< BR2                            */
#define CMD_BR3_BIT					0x0008		/**< BR3                            */
#define CMD_NT2R_BR_BIT				0x0003		/**< BR0 BR1						*/
#define CMD_NT4R_BR_BIT				0x000F		/**< BR0 BR1 BR2 BR3				*/
#define CMD_NT8R_BR_BIT				0x00FF		/**< BR0 BR1 BR2 BR3 ... BR7		*/
#define CMD_BR2_BR3_BIT				0x000C
#define CMD_NT4R_BR_MIN_BIT			0x0000		/**< 								*/

//TD-SRE update



/* @} */


/* ======================================================================== */
/* BR番号定義																*/
/* ======================================================================== */
#define CMD_BR0					0x00		/**< BR0							*/
#define CMD_BR1					0x01		/**< BR1							*/
#define CMD_BR2					0x02		/**< BR2							*/
#define CMD_BR3					0x03		/**< BR3							*/
#define CMD_MAX_BRNUM			4

/*TDD SRE  TRX setting S. */
#define CMD_MAX_BRNUM_NT4R		0x04		/**< NT4R BR数				*/		
#define CMD_MAX_BRNUM_NT2R		0x02		/**< NT2R BR数				*/	
/*TDD SRE  TRX setting E. */
#define CMD_BR_ONE_BIT_ON		0x01		/**< Single BR bit on  				*/			

/* @} */

/** @name TDL/MKM common used definition */
/* @{ */

/*	TDL/MKM common used definition	*/
#define CMD_RES_RE_TRASTAERR		0xFF000001	/* TRA Status error	*/
#define CMD_RES_RE_TRAUNINS			0xFF000002	/* TRA uninstall	*/
#define CMD_TRAFILE_MAX				16777216	/* Maximum size of the TRA file size*/
#define CMD_RCV_MPU_NUM				3145728		/* the number of the MPU size(3M) */

/* @} */

/** @name index value for TILT Send/Receive Counter (cmw_TILTRSCount) */
/* @{ */

/* ======================================================================== */
/* TILT Send/Receive Counter												*/
/* ======================================================================== */
#define CMD_RETILTSnd	0	/**< counter number for TILT Send status		*/
#define CMD_RETILTRec	1	/**< counter number for TILT Receive status		*/

/* @} */


/** @name RF share part state-use/unuse (cmw_trxset_tbl) */
/* @{ */

#define CMD_RF_PART_USE			0		/**< RF share part be usable		*/
#define CMD_RF_PART_UNUSE		1		/**< RF share part be unusable		*/

/* @} */

/** @name Task state (cmw_tskstamng_tbl) */
/* @{ */

/* ======================================================================== */
/* タスク状態																*/
/* ======================================================================== */
#define CMD_TSK_STATE_INI		0			/**< 初期状態					*/
#define CMD_TSK_STATE_1ST		1			/**< 初期診断待ち状態			*/
#define CMD_TSK_STATE_USE 		2			/**< 通常運用状態				*/

/* @} */

/** @name Client Info (cmw_tblClientInfo) */
/* @{ */
#define CMD_MX_MODE				3			/* 3 modes: OPT, simulator and monitor*/
#define CMD_MX_TCPNO			3			/* Max TCP amount       	*/

#define CMD_TCPNO_SIM			0			/* TCP link no : Simulator  */
#define CMD_TCPNO_MON			1			/* TCP link no : Monitor    */
#define CMD_TCPNO_REMT			2			/* TCP link no : REMT    	*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#ifndef OPT_RRH_ZYNQ_REC
typedef enum
{
    CMD_TCPCONN_NUM_SIMNO_REC,
    CMD_TCPCONN_NUM_SIMNO_RE01_06,
    CMD_TCPCONN_NUM_SIMNO_RE07_12,
    CMD_TCPCONN_NUM_SIMNO_RE13_16,
    CMD_TCPCONN_NUM_MONNO_REC,
    CMD_TCPCONN_NUM_MONNO_RE01_06,
    CMD_TCPCONN_NUM_MONNO_RE07_12,
    CMD_TCPCONN_NUM_MONNO_RE13_16,
    CMD_TCPCONN_NUM_FHMMT,
    CMD_TCPCONN_NUM_MAX
} T_CMD_TCPCONN_NUM;
#else
typedef enum
{
    CMD_TCPCONN_NUM_SIMNO_REC,
	CMD_TCPCONN_NUM_SIMNO_RE01,
    CMD_TCPCONN_NUM_SIMNO_RE02,
    CMD_TCPCONN_NUM_SIMNO_RE03,
    CMD_TCPCONN_NUM_SIMNO_RE04,
    CMD_TCPCONN_NUM_SIMNO_RE05,
    CMD_TCPCONN_NUM_SIMNO_RE06,
    CMD_TCPCONN_NUM_SIMNO_RE07,
    CMD_TCPCONN_NUM_SIMNO_RE08,
    CMD_TCPCONN_NUM_SIMNO_RE09,
    CMD_TCPCONN_NUM_SIMNO_RE10,
    CMD_TCPCONN_NUM_SIMNO_RE11,
    CMD_TCPCONN_NUM_SIMNO_RE12,
    CMD_TCPCONN_NUM_SIMNO_RE13,
    CMD_TCPCONN_NUM_SIMNO_RE14,
    CMD_TCPCONN_NUM_SIMNO_RE15,
    CMD_TCPCONN_NUM_SIMNO_RE16,
    CMD_TCPCONN_NUM_MONNO_REC,
	CMD_TCPCONN_NUM_MONNO_RE01,
    CMD_TCPCONN_NUM_MONNO_RE02,
    CMD_TCPCONN_NUM_MONNO_RE03,
    CMD_TCPCONN_NUM_MONNO_RE04,
    CMD_TCPCONN_NUM_MONNO_RE05,
    CMD_TCPCONN_NUM_MONNO_RE06,
    CMD_TCPCONN_NUM_MONNO_RE07,
    CMD_TCPCONN_NUM_MONNO_RE08,
    CMD_TCPCONN_NUM_MONNO_RE09,
    CMD_TCPCONN_NUM_MONNO_RE10,
    CMD_TCPCONN_NUM_MONNO_RE11,
    CMD_TCPCONN_NUM_MONNO_RE12,
    CMD_TCPCONN_NUM_MONNO_RE13,
    CMD_TCPCONN_NUM_MONNO_RE14,
    CMD_TCPCONN_NUM_MONNO_RE15,
    CMD_TCPCONN_NUM_MONNO_RE16,
    CMD_TCPCONN_NUM_FHMMT,
    CMD_TCPCONN_NUM_MAX
} T_CMD_TCPCONN_NUM;
#endif
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_CONN_Y				1			/* connected 				*/
#define CMD_CONN_N				0			/* not connected			*/

#define CMD_TCPHDL_NULL			-1			/* TCP handle null      	*/
#define CMD_TCPSVR_RST			10		  	/* TCP Server restart		 */
/* @} */

#define CMD_S3G_FREQ_MIN		225		/**< S3G freq no minimun		*/
#define CMD_S3G_FREQ_MAX		375		/**< S3G freq no maximun		*/
#define CMD_S3G_FREQ_800M_MIN	6025	/**< S3G-800MHz freq no minimun		*/
#define CMD_S3G_FREQ_800M_MAX	6125	/**< S3G-800MHz freq no maximun		*/
#define CMD_S3G_FREQ_15G_MIN	6475	/**< S3G-1.5GHz freq no minimun		*/
#define CMD_S3G_FREQ_15G_MAX	6575	/**< S3G-1.5GHz freq no maximun		*/

/* APD -register PU command setting value define */
#define CMD_APDBUF_VAL_CAR_PWR_3277	0x0CCD
#define CMD_APDBUF_VAL_CAR_PWR_2314 0x090A

/*!  
 * @name bandwidth pattern
 * @{  
 */
#define CMD_BADWID_NONE			0           /* no bandwidth */
#define CMD_BADWID_5m			1           /* 5m bandwidth */
#define CMD_BADWID_10m			2           /* 10m bandwidth */
#define CMD_BADWID_15m			3           /* 15m bandwidth */
#define CMD_BADWID_20m			4           /* 20m bandwidth */
#define CMD_BADWID_MAX			5           /* bandwidth最大 */
/* @} */

/*!  
 * @name define for soft version mask
 * @{  
 */
#define CMD_MJRVER_MSK		0xFF00				/* メジャーバージョン用マスク	*/
#define CMD_MINVER_MSK		0x00FF				/* メジャーバージョン用マスク	*/
#define CMD_VER_MSK			0x000F				/* ソフトバージョン用マスク		*/
#define CMD_BIT_MSK			0x0001				/* the mask of Bit Line Rate 	*/
/* @} */


/*!  
 * @name the UART ID NO
 * @{  
 */
#define CMD_UART_NO0         0
#define CMD_UART_NO1         1
#define CMD_UART_NO2         2
/* @} */

/*!  
 * @name the TRX set - syspara check NG code
 * @{  
 */
#define CMD_SYSPARA_CHECKNG					 2

/* @} */

/*!  
 * @name the TRX set - S3G/3G freq check NG code , 0-224 and 376-599, NG=41 
 * @{  
 */
#define CMD_S3G_FREQ_CHECKNG				 3
#define CMD_3G_FREQ_CHECKNG				 	 3
/* @} */

/*!  
 * @name the TRX set - S3G Bandwith check NG code , 0-224 and 376-599, NG=41 
 * @{  
 */
#define CMD_S3G_BW_CHECKNG				 	 4
/* @} */

/*!  
 * @name the TRX set - S3G Send branch check NG code , 0-224 and 376-599, NG=41 
 * @{  
 */
#define CMD_S3G_SNDBR_CHECKNG				 5
/* @} */

/*!  
 * @name the DLM and TDL racing flag
 * @{  
 */
#define CMD_DLM_ON					 1
#define CMD_TDL_ON					 2

/* @} */

/*!  
 * @周波数番号 
 * @{  
 */
#define CMD_FRQ2G_MIN			9612	/**<  周波数番号 2G					*/
#define CMD_FRQ2G_MAX			9888	/**<  周波数番号 2G					*/
#define CMD_FRQ800M_1_MIN		4162	/**<  周波数番号 800M				*/
#define CMD_FRQ800M_1_MAX		4188	/**<  周波数番号 800M				*/
#define CMD_FRQ800M_2_MIN		339		/**<  周波数番号 800M				*/
#define CMD_FRQ800M_2_MAX		363		/**<  周波数番号 800M				*/
#define CMD_FRQ800M_3			812		/**<  周波数番号 800M				*/
#define CMD_FRQ800M_4			837		/**<  周波数番号 800M				*/
#define CMD_FRQ800M_5			437		/**<  周波数番号 800M				*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_FRQ17G_MIN			8562	/**<  周波数番号 17G				*/
#define CMD_FRQ17G_MAX			8913	/**<  周波数番号 17G				*/
#define CMD_FRQ15G_MIN			3487	/**<  周波数番号 15G				*/
#define CMD_FRQ15G_MAX			3587	/**<  周波数番号 15G				*/

#define CMD_FRQ17G_LTE_MIN		1774	/**<  周波数番号 17G				*/
#define CMD_FRQ17G_LTE_MAX		1924	/**<  周波数番号 17G				*/
#define CMD_FRQ2G_LTE_MIN		225		/**<  周波数番号 2G					*/
#define CMD_FRQ2G_LTE_MAX		375		/**<  周波数番号 2G					*/
#define CMD_FRQ15G_LTE_MIN		6475	/**<  周波数番号 15G				*/
#define CMD_FRQ15G_LTE_MAX		6575	/**<  周波数番号 15G				*/
#define CMD_FRQ800M_LTE_MIN		6025	/**<  周波数番号 700M				*/
#define CMD_FRQ800M_LTE_MAX		6125	/**<  周波数番号 700M				*/
#define CMD_FRQ700M_LTE_MIN		9485	/**<  周波数番号 700M				*/
#define CMD_FRQ700M_LTE_MAX		9535	/**<  周波数番号 700M				*/
#define CMD_FRQ35G_LTE_MIN		42415	/**<  周波数番号 35G				*/
#define CMD_FRQ35G_LTE_MAX		42765	/**<  周波数番号 35G				*/
#define CMD_FRQ34G_LTE_MIN		42015	/**<  周波数番号 34G 				*//* ハソ提供後に見直しが必要 */
#define CMD_FRQ34G_LTE_MAX		42365	/**<  周波数番号 34G				*//* ハソ提供後に見直しが必要 */

#define CMD_UPDWN_LNK_MIN		0		/**<  				*/
#define CMD_UPDWN_LNK_MAX		6		/**<  				*/
#define CMD_SPE_SF_MIN			0		/**<  				*/
#define CMD_SPE_SF_MAX			9		/**<  				*/
#define CMD_DWNLK_CP_MIN		0		/**<  				*/
#define CMD_DWNLK_CP_MAX		1		/**<  				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/** @name Max data command size */
/* @{ */
#define CMD_MAX_COMMAND_SIZ		10240		/**< 最大送信サイズ	data command	*/
/* @} */

/** @name Max data file size */
/* @{ */
#define CMD_MAX_FILE_SIZ		4096		/**< 最大送信サイズ	data file		*/
/* @} */

/** @name Negative cause */
#define CMD_ACTNEG_CHG_CAUSE_BLK		0		/**< BLK要因	*/
#define CMD_ACTNEG_CHG_CAUSE_INSALM	1		/**< 実装+ALM要因	*/
/* @} */




/**********/
/* マクロ */
/**********/
/*!
 * @brief  ビット設定
 * @{  
 */
#define CMM_SETBIT(field, mask, onoff) {\
            if (onoff == 0) {field &= ~mask;} \
            else            {field |=  mask;} \
        }
/* @} */

/*!
 * @brief  ビット取得
 * @{  
 */
#define CMM_GETBIT(field, mask) \
            ((field & mask)/mask)
/* @} */

/*!
 * @brief スリープ状態 
 * @{  
 */
/* スリープ状態 - 各ブランチのSLP状態 */
#define CMD_SLPSTATE_NOSLP		0		/* SLP状態ではない */
#define CMD_SLPSTATE_SLPSET		1		/* SLP制御安定化時間中 */
#define CMD_SLPSTATE_SLP		2		/* SLP状態 */
#define CMD_SLPSTATE_SLPREL		3		/* SLP解除安定化時間中 */
#define CMD_SLPSTATE_ALM		4		/* ALM */

#define CMD_PASTATE_OFF			0		/* PA-OFF*/
#define CMD_PASTATE_ON			1		/* PA-ON */
#define CMD_PASTATE_OFFSTBL		2		/* PA-OFF安定化中 */
#define CMD_PASTATE_ONSTBL		3		/* PA-ON安定化中 */


#define CMD_SLP_BR_0	1		/* 0 系スリープ */
#define CMD_SLP_BR_1	2		/* 1 系スリープ */
#define CMD_SLP_BR_0_1	3		/* 0 系/1 系スリープ */


/* @} */
/****************************************************************************/
/* 3G BTS share type														*/
/****************************************************************************/
#define CMD_CARDSV_ERR_BIT		0x0100
#define CMD_CARDSV_ALM_BIT		0x0200
#define CMD_CARDSV_ACT_BIT		0x0020
#define CMD_CARDSV_BLK_BIT		0x0002
#define CMD_CARDSV_BLK_BIT_3G	0x0004

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_CARDSV_IMP_BIT		0x0080
#define CMD_CARDSV_USE_BIT		0x0400
#define CMD_CARDSV_S3G_USE_BIT  0x000F
#define CMD_CARDSV_3G_USE_BIT   0x00F0
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define CMD_ACT_BIT				0x2000		/* カードACTビット				*/
#define CMD_ALM_BIT_ON			0x0002		/* ALM bit set ON value in SV A0/A1 */
#define CMD_ERR_BIT_ON			0x0001		/* ERR bit set ON value in SV A0/A1 */
#define CMD_BLK_BIT_ON			0x0400		/* BLK bit set ON value in SV A0/A1 */
#define CMD_CHK_BIT_ON			0x0200		/* CHK bit set ON value in SV A0/A1 */
#define CMD_USE_BIT_ON			0x0004		/* USE bit set ON value in SV A0/A1 */
#define CMD_PA_ERR_BIT			0x0300		/* PA-OFF ERR bit in SV A6/A7*/
#define CMD_CLR_ERRBIT1			0xFFFE		/**< Bit 1 clear	(ERR)	*/
#define CMD_NOTINS_BIT_ON		0x4000
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define CMD_MODEL3G_MAX				 2
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/*!
 * @brief FPAG register range
 * @{  
 */
#define CMD_REG_9 				0x90000000
#define CMD_REG_9_SIZE 			0x5F600000
/* @} */

/*!
 * @brief CPLD register range
 * @{  
 */
#define CMD_REG_F 				0xF1000000
#define CMD_REG_F_SIZE 			0x00000100
/* @} */

/*!
 * @brief hardware alarm type
 * @{  
 */
#define CMD_HWALMTYPE_PLL		0
#define CMD_HWALMTYPE_CPRI		1
#define CMD_HWALMTYPE_APDCOM	2
#define CMD_HWALMTYPE_APD0		3
#define CMD_HWALMTYPE_APD1		4
/* @} */

/*!
 * @brief hardware alarm detect type
 * @{  
 */
#define CMD_ALMDETECT_SW		0x0001
#define CMD_ALMDETECT_HW		0x0002
/* @} */

#define CMD_HARDALM_MAX			23
#define CMD_LEDSET_MAX			28
#define CMD_LEDSET_MAX_15G17GLRE	48

/*!
 * @brief SPI device type
 * @{  
 */
#define CMD_SPITYP_EEPROM		0		/* EERPOM */
#define CMD_SPITYP_AD9523		1		/* AD9523 */
#define CMD_SPITYP_RXADC		2		/* RX-ADC */
#define CMD_SPITYP_FBADC		3		/* FB-ADC */


/*!
 * @brief SPI device type
 * @{  
 */
#define CMD_AMPDISC_BIT			0x0010		/**< AMPｶﾏERR bit in SV A8/A9		*/
#define CMD_SLP0_ERR_BIT		0x0020		/**< SLP0 ERR bit in SV A8/A9		*/
#define CMD_SLP1_ERR_BIT		0x0040		/**< SLP1 ERR bit in SV A8/A9		*/
#define CMD_SLP0_1_ERR_BIT	(CMD_SLP0_ERR_BIT |CMD_SLP1_ERR_BIT)
//add SLP 2 3 TDD-SRE
#define CMD_SLP2_ERR_BIT		0x0080		/**< SLP2 ERR bit in SV A8/A9		*/
#define CMD_SLP3_ERR_BIT		0x0100		/**< SLP3 ERR bit in SV A8/A9		*/
#define CMD_SLP2_3_ERR_BIT  ( CMD_SLP2_ERR_BIT | CMD_SLP3_ERR_BIT )
//add  SLP 2 3  TDD-SRE
#define CMD_SLPAMP_BIT_NT2R	(CMD_AMPDISC_BIT | CMD_SLP0_ERR_BIT | CMD_SLP1_ERR_BIT)
#define CMD_SLPAMP_BIT_NT4R	(CMD_AMPDISC_BIT | CMD_SLP0_ERR_BIT | CMD_SLP1_ERR_BIT |CMD_SLP2_ERR_BIT |CMD_SLP3_ERR_BIT)
/* @} */


#define CMD_FAN_ALM1_BIT			0x0040		
#define CMD_FAN_ALM2_BIT			0x0080
#define CMD_FAN_ALM3_BIT			0x4000
#define CMD_FAN_ALM4_BIT			0x8000
#define CMD_CASCADE_LINK_ERR_BIT 	0x0020
#define CMD_OFFSET_DATA				0x0001
#define CMD_FAN1_ALM_MAST			0xFF7F
#define CMD_FAN2_ALM_MAST			0xFFBF
#define CMD_CASCADE_LINK_MAST		0xFFDF



/*!
 * @brief branch status type
 * @{  
 */
#define CMD_BRSTA_NORMAL		0x0000		/* Normal	*/
#define CMD_BRSTA_STOP			0x0001		/* Stop		*/
#define CMD_BRSTA_ALM			0x0002		/* Alarm	*/
//TD-SRE
#define CMD_BRSTA_NOTINS		0x0003		/* NOTINS*/




/* @} */

/** @name TRA SV state definition (cmw_tra_sv_tbl)*/
/* @{ */

/* ======================================================================== */
/* TRA SV状態管理テーブル													*/
/* ======================================================================== */

#define CMD_REGADR_MDIF_SV		0x90000300		/**< MDIF SVレジスタアドレス*/
#define CMD_REGADR_SVCTL1		0x90000310		/* SV#1 Ctrl setting register */
#define CMD_REGADR_SVCTL3		0x90000318		/* SV#3 Ctrl setting register */
/* @} */	
	
/* テーブル読出し用デファイン	*/
/**< REG SV A0		*/
#define CMD_TRA_REG_USE			0x0004	/**< USE								*/
#define CMD_TRA_REG_ALM			0x0002	/**< ALM								*/
#define CMD_TRA_REG_ERR			0x0001	/**< ERR								*/

/**< REG SV A1		*/
#define CMD_TRA_REG_INS			0x8000	/**< 実装								*/
#define CMD_TRA_REG_NOINS		0x4000	/**< 未実装								*/
#define CMD_TRA_REG_ACT			0x2000	/**< ACT								*/
#define CMD_TRA_REG_BLK			0x0400	/**< BLK								*/
#define CMD_TRA_REG_TST			0x0200	/**< 診断								*/
#define CMD_TRA_REG_CONST		0x0100	/**< Card in Construction				*/


/**< REG SV A2		*/
#define CMD_TRA_REG_BR_USE_BIT		0x0001


#define CMD_TRA_REG_BRALM0		0x0001	/**< BR#0 ALM							*/
#define CMD_TRA_REG_BRALM1		0x0010	/**< BR#1 ALM							*/
//TD-SRE
#define CMD_TRA_REG_BRALM2		0x0100	/**< BR#2 ALM							*/
#define CMD_TRA_REG_BRALM3		0x1000	/**< BR#3 ALM							*/
//TD-SRE

#define CMD_TRA_SV_NUM			5		/**< TRA SVバス情報数					*/


/**
 * @brief	温度補正値
 */
#define CMD_TEM_VAL_MIN_R	(-40)					/* 温度最小値(本来)(℃) */
#define CMD_TEM_VAL_MAX_R	125						/* 温度最大値(本来)(℃) */
#define CMD_TEM_VAL_ADJ		(-CMD_TEM_VAL_MIN_R)	/* 温度補正値(℃) */

/**
 * @brief	範囲＆異常
 */
#define CMD_TEM_VAL_MIN		(CMD_TEM_VAL_MIN_R+CMD_TEM_VAL_ADJ)
													/* 温度最小値(℃) */
#define CMD_TEM_VAL_MAX		(CMD_TEM_VAL_MAX_R+CMD_TEM_VAL_ADJ)
													/* 温度最大値(℃) */
#define CMD_TEM_VAL_ABN		((SHORT)0xFFFF)			/* 温度異常値(℃) */
#define CMD_HUM_VAL_MIN		0						/* 湿度最小値(％) */
#define CMD_HUM_VAL_MAX		100						/* 湿度最大値(％) */
#define CMD_HUM_VAL_ABN		((SHORT)0xFFFF)			/* 湿度異常値(％) */
#define CMD_CUR_VAL_MIN		0						/* 電流最小値(1/10^2mV) */
#define CMD_CUR_VAL_MAX		8192					/* 電流最大値(1/10^2mV) */
#define CMD_CUR_VAL_ABN		((SHORT)0xFFFF)			/* 電流異常値(0.1A) */
#define CMD_VOL_VAL_MIN		70						/* 電圧最小値(0.1V) */
#define CMD_VOL_VAL_MAX		800						/* 電圧最大値(0.1V) */
#define CMD_VOL_VAL_ABN		((SHORT)0xFFFF)			/* 電圧異常値(0.1V) */


/**
 * @brief	測定結果
 */
enum {
	CMD_MSR_OK,				/* 0x00000000:OK */
	CMD_MSR_NG,				/* 0x00000001:異常終了(値はNGを示す値に補正) */
	CMD_MSR_RANGE_OVER		/* 0x00000002:範囲外(値は上限/下限に補正) */
};

/****************************************************************************/
/* 信号種別																	*/
/****************************************************************************/
enum
{
	CMD_SIGKIND_3G = 0,							/* 3G								*/
	CMD_SIGKIND_LTE,							/* LTE								*/
	CMD_SIGKIND_MAX
};

/****************************************************************************/
/* 接続種別																	*/
/****************************************************************************/
enum
{
	CMD_CONKIND_3G_SINGLE = 0,					/* 3G単独							*/
	CMD_CONKIND_LTE_SINGLE,						/* LTE単独							*/
	CMD_CONKIND_BOTH,							/* 3G/LTE両系接続					*/
	CMD_CONKIND_MAX
};
enum
{
	CMD_LINE_BITRATE_UNKNONWN = 0,
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	CMD_LINE_BITRATE_24G,                       /**< 2.4G */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	CMD_LINE_BITRATE_49G,						/* 4.9G						*/
	CMD_LINE_BITRATE_98G						/* 9.8G						*/
};

/****************************************************************************/
/* シスパラ関連																*/
/****************************************************************************/
#define CMD_SYS_SEC_N_CHIP_MAX		16			/* セクタnチップ最大値				*/

/****************************************************************************/
/* 周波数番号																*/
/****************************************************************************/
#define CMD_FREQ_2G_LTE_MIN			225			/* 2G SRE LTE周波数番号MIN		*/
#define CMD_FREQ_2G_LTE_MAX			375			/* 2G SRE LTE周波数番号MAX		*/
#define CMD_FREQ_700M_LTE_MIN		9485		/* 700M SRE LTE周波数番号MIN		*/
#define CMD_FREQ_700M_LTE_MAX		9535		/* 700M SRE LTE周波数番号MAX		*/
#define CMD_FREQ_800M_LTE_MIN		6025		/* 800M SRE LTE周波数番号MIN		*/
#define CMD_FREQ_800M_LTE_MAX		6125		/* 800M SRE LTE周波数番号MAX		*/
#define CMD_FREQ_15G_LTE_MIN		6475		/* 1.5G RRE LTE周波数番号MIN		*/
#define CMD_FREQ_15G_LTE_MAX		6575		/* 1.5G RRE LTE周波数番号MAX		*/
#define CMD_FREQ_17G_LTE_MIN		1774		/* 1.7G RRE LTE周波数番号MIN		*/
#define CMD_FREQ_17G_LTE_MAX		1924		/* 1.7G RRE LTE周波数番号MAX		*/
#define CMD_FREQ_35G_LTE_MIN		42415		/* 3.5G SRE LTE周波数番号MIN		*/
#define CMD_FREQ_35G_LTE_MAX		42765		/* 3.5G SRE LTE周波数番号MAX		*/

/****************************************************************************/
/* RE-MT関連																*/
/****************************************************************************/
#define CMD_REMT_SENDPOW_UP			0			/* 送信出力UP						*/
#define CMD_REMT_SENDPOW_DOWN		1			/* 送信出力DOWN						*/
#define CMD_REMT_FREQ_GET_TRX		0			/* 周波数番号取得要求				*/
#define CMD_REMT_FREQ_GET_NOMOD		1			/* 送信電力取得要求					*/
#define CMD_REMT_SENDPOW_5UP		2			/* LRE送信出力5UP					*/
#define CMD_REMT_SENDPOW_5DOWN		3			/* LRE送信出力5DOWN					*/
#define CMD_REMT_SENDPOW_CMDMAX		4			/* 送信出力コマンド最大値			*/
#define CMD_REMT_SENDPOW_READ		CMD_REMT_SENDPOW_5UP	/* LRE送信出力読出		*/

#define CMD_SENDPOW_INCVAL			1			/* 送信出力インクリメント値			*/
#define CMD_SENDPOW_MINVAL			0			/* 送信出力下限値					*/
#define CMD_SENDPOW_MAXVAL			10			/* 送信出力上限値					*/
#define CMD_SENDPOW_INCLRE			5			/* LRE送信出力インクリメント値		*/
#define CMD_SENDPOW_MINLRE			0			/* 送信出力下限値					*/
#define CMD_SENDPOW_MAXLRE			30			/* 送信出力上限値					*/

/****************************************************************************/
/* POW計算																	*/
/****************************************************************************/
#define CMD_CALC_POW10RESULT		100000000	/* POW10後の少数点合わせ用			*/
/*TDD SRE  TRX setting S. */
/*!  
 * @name carrier pattern
 * @{  
 */
enum
{
	CMD_CARNO_C1 = 1,							/* Carrier#1	*/
	CMD_CARNO_C2								/* Carrier#2	*/
};
/*TDD SRE  TRX setting E. */
#define SOCK_INVALID 			-1			/*Invalid socket handle*/
#define SOCK_VALID				0
#define SOCK_RES_OK				0			/*socket function return OK*/
#define SOCK_SEL_TO				0			/*socket select function return timeout*/
#define SOCK_SEL_OK				1			/*socket select function return ok*/	
#define SOCK_RD_OK				1			/*socket recv function return ok*/	
#define SOCK_SHUT_ALL			2			/*shutdown socket read and send*/
#define SOCK_SHUT_RD			0			/*shutdown socket read */
#define SOCK_SHUT_WR			1			/*shutdown socket read */
#define SOCK_DEFBACKLOG			5			/*default pending connections*/

#define	CMD_DEVKND_RRE_MAX		4		/**< device kind for RRE 				*/

/* ======================================================================== */
/* LED control register														*/
/* ======================================================================== */
#define CMD_LED_COND_INS 			0x01
#define CMD_LED_COND_ACT			0x02
#define CMD_LED_COND_BLK 			0x04
#define CMD_LED_COND_USE 			0x08
#define CMD_LED_COND_ALM_ALL 		0x10
#define CMD_LED_COND_ALM_BR 		0x20
#define CMD_LED_COND_ALM_BRS 		0x40
#define CMD_LED_COND_ACTIVE 		0x80

#define CMD_LED_COND_DWL 			0x0100
#define CMD_LED_COND_BSY 			0x0200

#define CMD_LED_DARK			0x0000
#define CMD_LED_LIGHT			0x0001
#define CMD_LED_LIGHT_CYCLE0	0x0002
#define CMD_LED_LIGHT_CYCLE1	0x0003
#define CMD_LED_DL_OFF			0x0004
/*TDD SRE  TRX setting S. */
/* NT2R:0 NT4R:1 */
enum
{
	CMD_DEV_TYPE_NT2R = 0,					/* NT2R					*/
	CMD_DEV_TYPE_NT4R,						/* NT4R					*/
	CMD_DEV_TYPE_NT8R						/* NT8R					*/
};

/* branch status alrm:0,use:1 */
enum
{
	CMD_BR_ALM = 0,				/* use					*/
	CMD_BR_USE					/* alarm				*/
};

/* Function return code, check OK:0, check NG:1, para error:2 */
enum
{
	CMD_RT_NG = 0,				
	CMD_RT_OK,					
	CMD_RT_PARA_CHC_NG				
};
/*TDD SRE  TRX setting  E. */
/* @} */
#endif	/* M_CM_DEF */

