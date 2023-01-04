/*!
 *	@skip	$ld:$
 *	@file	f_trp_com_def.h
 *	@brief	TRP common define header
 *	@date	2007/03/20	FFCS)linjunhua
 *	@date	2008/07/29	FFCS)Tangj	S3G PRC eNB-008-002
 *	@date	2010/01/05	QNET)Nagasima	M-S3G-eNBPF-02937.
 *	@date	2010/01/20	QNET)Nagasima	M-S3G-eNBPF-02937.
 *	@date   2010/09/21	CMS)Takahashi modify for CR***
 *	@date   2010/10/22	FJT)Koshida modify for eNB-004 of TS656案5対応
 *	@date	2010/11/05	FJT)Tokunaga	M-S3G-eNBSYS-xxxxx : TS-810項
 *  @date   2010/11/18	FJT)Koshida modify for M-S3G-eNBSYS-02100;☆ST_ENB☆TS841項：CPRI-MUX含まない再開を実施した際に3G側にてBRE ALM検出
 *  @date   2011/11/10 FJT)K.Koshida D-MICRO-008 3002形対応(LAPB削除,H/S inf変更対応)
 *  @date   2012/01/24 FJT)K.Koshida modify for CR-HARD-00008
 *  @date   2016/02/25 TDI)satou TRX set request confirm timer 200msから180msに
 *  @date   2016/02/25 TDI)satou TRX release request confirm timer 200msから180msに
 *	
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2007-2012
 */
#ifndef _F_TRP_COM_DEF_h
#define _F_TRP_COM_DEF_h

/*! @addtogroup TRIF_TCM
 * @{  */

#define	F_COM_CMM_FUNCRACE(...)
#define	F_COM_CMM_FANCRACE(...)

///* ======================================================================== */
///* スレッド内部番号定義														*/
///* ======================================================================== */
//#define	D_TCM_THRINO_MIN		0x0000		/* スレッド内部番後MIN			*/
//#define	D_TCM_THRINO_TRM		0x0000		/* TRIFメイン					*/
//#define	D_TCM_THRINO_TRO		0x0001		/* TRIF OCM						*/
#define	D_TCM_THRINO_CPC		0x0002		/* CPRI制御						*/
#define	D_TCM_THRINO_REC		CMD_TSKID_REC	/* RE制御						*/
//#define	D_TCM_THRINO_RED		0x0004		/* REダウンロード				*/
//#define	D_TCM_THRINO_EDT		0x0005		/* 外部データ転送				*/
//#define	D_TCM_THRINO_CMC		0x0006		/* CPRI-MT制御					*/
#define	D_TCM_THRINO_LPB		0x0007		/* LAPB							*/
//#define	D_TCM_THRINO_TXS		0x0008		/* TXIOCS						*/
//#define	D_TCM_THRINO_RXS		0x0009		/* RXIOCS						*/
//#define	D_TCM_THRINO_MTR		0x000A		/* FLD-MT/SV-MT Data Transfer 	*/
#define	D_TCM_THRINO_RCT		0x000B		/* RCT 	*/
#define	D_TCM_THRINO_MAX		0x000C		/* スレッド内部番号MAX			*/

/* ======================================================================== */
/* スレッド外部番号定義														*/
/* ======================================================================== */
//#define	D_TCM_THRENO_MIN		0x1000		/* スレッド外部番後MIN			*/
//#define	D_TCM_THRENO_TRM		0x1000		/* TRIFメイン					*/
//#define	D_TCM_THRENO_TRO		0x1001		/* TRIF OCM						*/
//#define	D_TCM_THRENO_CPC		0x1002		/* CPRI制御						*/
#define	D_TCM_THRENO_REC		0x1003		/* RE制御						*/
//#define	D_TCM_THRENO_RED		0x1004		/* REダウンロード				*/
//#define	D_TCM_THRENO_EDT		0x1005		/* 外部データ転送				*/
//#define	D_TCM_THRENO_CMC		0x1006		/* CPRI-MT制御					*/
//#define	D_TCM_THRENO_LPB		0x1007		/* LAPB							*/
//#define	D_TCM_THRENO_TXS		0x1008		/* TXIOCS						*/
//#define	D_TCM_THRENO_RXS		0x1009		/* RXIOCS						*/
//#define	D_TCM_THRENO_MTR		0x100A		/* FLD-MT/SV-MT Data Transfer 	*/
//#define	D_TCM_THRENO_MAX		0x100B		/* スレッド外部番号MAX			*/
//
///* ======================================================================== */
///* スレッドキュー番号定義													*/
///* ======================================================================== */
//#define	D_TCM_THRQNO_MIN		0x0000	/* MessageQ number MIN				*/
//#define	D_TCM_THRQNO_TRM_NML	0x0000	/* TRIFメイン(通常)					*/
//#define	D_TCM_THRQNO_TRO_NML	0x0001	/* TRIF OCM(通常)					*/
//#define	D_TCM_THRQNO_CPC_NML	0x0002	/* CPRI制御(通常)					*/
//#define	D_TCM_THRQNO_REC_NML	0x0003	/* RE制御(通常)						*/
//#define	D_TCM_THRQNO_RED_NML	0x0004	/* REダウンロード(通常)				*/
//#define	D_TCM_THRQNO_EDT_NML	0x0005	/* 外部データ転送(通常)				*/
//#define	D_TCM_THRQNO_CMC_NML	0x0006	/* CPRI-MT制御(通常)				*/
//#define	D_TCM_THRQNO_LPB_NML	0x0007	/* LAPB(通常)						*/
//#define	D_TCM_THRQNO_TXS_NML	0x0008	/* TXIOCS(通常)						*/
//#define	D_TCM_THRQNO_RXS_NML	0x0009	/* RXIOCS(通常)						*/
//#define	D_TCM_THRQNO_MTR_NML	0x000A	/* MTR(通常) 						*/
//#define	D_TCM_THRQNO_LOGC_NML	0x000D	/* LOGC (Common part)				*/
//#define	D_TCM_THRQNO_MAX		0x000E	/* MessageQ number MAX (TRIF threads + Common part) */
//
///* ======================================================================== */
///* スレッドプライオリティ定義												*/
///* ======================================================================== */
//#define	D_TCM_THRPRI_TRM		85			/* TRIFメイン					*/
//#define	D_TCM_THRPRI_TRO		85			/* TRIF OCM						*/
//#define	D_TCM_THRPRI_CPC		75			/* CPRI制御						*/
//#define	D_TCM_THRPRI_REC		75			/* RE制御						*/
//#define	D_TCM_THRPRI_RED		73			/* REダウンロード				*/
//#define	D_TCM_THRPRI_EDT		74			/* 外部データ転送				*/
//#define	D_TCM_THRPRI_CMC		70			/* CPRI-MT制御					*/
//#define	D_TCM_THRPRI_LPB		80			/* LAPB							*/
//#define	D_TCM_THRPRI_TXS		81			/* TXIOCS						*/
//#define	D_TCM_THRPRI_RXS		81			/* RXIOCS						*/
//#define	D_TCM_THRPRI_MTR		72			/* MTR							*/
//
///* ======================================================================== */
///* 通信スレッド内部番号定義													*/
///* ======================================================================== */
//#define	D_TCM_CTHINO_MIN		0x0000		/**< 通信スレッド内部番号MIN	*/
//#define	D_TCM_CTHINO_CMTRCV		0x0000		/**<  CPRI-MT受信				*/
//#define	D_TCM_CTHINO_CMDRCV		0x0001		/**<  CPRI-MT受信(デバッグ)		*/
//#define	D_TCM_CTHINO_PRCRCV		0x0002		/**<  プロセス間受信			*/
//#define	D_TCM_CTHINO_MAX		0x0003		/**< 通信スレッド内部番号MAX	*/
//
///* ======================================================================== */
///* 通信スレッド外部番号定義													*/
///* ======================================================================== */
//#define	D_TCM_CTHRENO_CMTRCV    0x1010		/**<  CPRI-MT受信				*/
//#define D_TCM_CTHRENO_CMDRCV    0x1011      /**<  CPRI-MT受信(デバッグ)		*/
//#define D_TCM_CTHRENO_PRCRCV    0x1012      /**<  プロセス間受信			*/
//		
///* ======================================================================== */
///* 通信スレッドキュー番号定義												*/
///* ======================================================================== */
//#define	D_TCM_THRQNO_CMTRCV		0x00F0		/* CPRI-MT受信					*/
//#define	D_TCM_THRQNO_CMDRCV		0x00F1		/* CPRI-MT受信(デバッグ)		*/
//#define	D_TCM_THRQNO_PRCRCV		0x00F2		/* プロセス間受信				*/
//
///* ======================================================================== */
///* 通信スレッドプライオリティ定義											*/
///* ======================================================================== */
//#define	D_TCM_THRPRI_CMTRCV		85			/* CPRI-MT受信					*/
//#define	D_TCM_THRPRI_CMDRCV		85			/* CPRI-MT受信(デバッグ)		*/
//#define	D_TCM_THRPRI_PRCRCV		85			/* プロセス間受信				*/
//
///* ======================================================================== */
///* デバッグスレッド内部番号定義												*/
///* ======================================================================== */
//#define	D_TCM_DTHINO_MIN		0x0000		/* デバッグスレッド内部番号MIN	*/
//#define	D_TCM_DTHINO_DBG		0x0000		/* デバッグ					*/
//#define	D_TCM_DTHINO_MAX		0x0001		/* デバッグスレッド内部番号MAX	*/
//
///* ======================================================================== */
///* デバッグスレッドキュー番号定義											*/
///* ======================================================================== */
//#define	D_TCM_THRQNO_DBG		0x00E0		/* デバッグ						*/
//
///* ======================================================================== */
///* デバッグスレッドプライオリティ定義										*/
///* ======================================================================== */
//#define	D_TCM_THRPRI_DBG		15			/* デバッグ						*/
//
///* ======================================================================== */
///* 応答結果定義(汎用)														*/
///* ======================================================================== */
//
//#define	CMD_OK					 0
//#define	CMD_NG					 1
//#define	CMD_OAM_TO				 2
//#define	CMD_ON					 1
//#define	CMD_OFF					 0
//#define CMD_TRUE				 1
//#define CMD_FALSE				 0
//#define CMD_RES_OK       0   /* 正常終了      */
//
///****************************************************************************/
///* L3信号転送処理															*/
///****************************************************************************/
///* 復帰値 */
//#define	CMD_L3_OK				0		/* OK								*/
//#define	CMD_L3_CPRICUT			1		/* CPRIリンク断						*/
//#define	CMD_L3_CPRINOT			2		/* CPRIリンク存在しない				*/
//
//
///****************************************************************************/
///* 共通関数t_cm_CPRILnkJdg reference constant 								*/
///****************************************************************************/
///* CPRI link state number define											*/
//#define	CRD_CPRILNKSTA_NO0		0			/* CPRI stateE donw */
//											/* HDLC idle */
//											/* none-stateF */
//											/* TX abnormal */
//											/* L3 disconnection */
//
//#define	CRD_CPRILNKSTA_NO1		1			/* CPRI stateE */
//											/* HDLC idle */
//											/* none-stateF */
//											/* TX abnormal */
//											/* L3 disconnection */
//
//#define	CRD_CPRILNKSTA_NO2		2			/* CPRI stateE */
//											/* HDLC active */
//											/* none-stateF */
//											/* TX normal */
//											/* L3 disconnection */
//
//#define	CRD_CPRILNKSTA_NO3		3			/* CPRI stateE */
//											/* HDLC active */
//											/* none-stateF */
//											/* TX abnormal */
//											/* L3 disconnection */
//
//#define	CRD_CPRILNKSTA_NO4		4			/* CPRI stateE */
//											/* HDLC active */
//											/* stateF */
//											/* TX normal */
//											/* L3 disconnection */
//
//#define	CRD_CPRILNKSTA_NO5		5			/* CPRI stateE */
//											/* HDLC active */
//											/* stateF */
//											/* TX normal */
//											/* L3 OK */
//
///* ======================================================================== */
///* アサートログ出力レベル定数定義											*/
///* ======================================================================== */
//#define	CMD_ASL_NOLEVEL			0x00000000	/* レベル指定なし				*/
//#define	CMD_ASL_DBGLOW			0x00000002	/* デバッグ用低優先（通常使用）	*/
//#define	CMD_ASL_DBGHIGH			0x00000004	/* デバッグ用高優先（一時解析）	*/
//#define	CMD_ASL_USELOW			0x00000006	/* 運用低優先（フェールセーフ）	*/
//#define	CMD_ASL_USEHIGH			0x00000008	/* 運用高優先（ＲＡＳ）			*/
//
///* ======================================================================== */
//
//
///****************************************************************************/
///* 共通項目デファイン定義													*/
///****************************************************************************/
//#ifndef NULL
//#define	NULL					0			/* NULL							*/
//#endif
//#define	CMD_ALLF_CHAR			0xFF		/* ALLF初期化(CHAR)				*/
//#define	CMD_ALLF_SHORT			0xFFFF		/* ALLF初期化(SHORT)			*/
//#define	CMD_ALLF_INT			0xFFFFFFFF	/* ALLF初期化(INT)				*/
//#define	CMD_FLG_ON				1			/* フラグON						*/
//#define	CMD_FLG_OFF				0			/* フラグOFF					*/
//
//
///* Bit */
//#define	CMD_BIT1				1
//#define	CMD_BIT2				2
//#define	CMD_BIT3				3
//#define	CMD_BIT4				4
//#define	CMD_BIT5				5
//#define	CMD_BIT6				6
//#define	CMD_BIT7				7
//#define	CMD_BIT8				8
//#define	CMD_BIT9				9
//#define	CMD_BIT10				10
//#define	CMD_BIT11				11
//#define	CMD_BIT12				12
//#define	CMD_BIT13				13
//#define	CMD_BIT14				14
//#define	CMD_BIT15				15
//#define	CMD_BIT16				16
//
///* Byteサイズ */
//#define	CMD_BYTESIZE_INT		4
//
///****************************************************************************/
///* 共通項目デファイン定義(データ設定用)										*/
///****************************************************************************/
//#define	CMD_NUM_MINUS1			-1			/* -1							*/
//#define	CMD_NUM0				0			/* 0							*/
//#define	CMD_NUM1				1			/* 1							*/
//#define	CMD_NUM2				2			/* 2							*/
//#define	CMD_NUM3				3			/* 3							*/
//#define	CMD_NUM4				4			/* 4							*/
//#define	CMD_NUM5				5			/* 5							*/
//#define	CMD_NUM6				6			/* 6							*/
//#define	CMD_NUM7				7			/* 7							*/
//#define	CMD_NUM8				8			/* 8							*/
//#define	CMD_NUM9				9			/* 9							*/
//#define	CMD_NUM10				10			/* 10							*/
//#define	CMD_NUM11				11			/* 11							*/
//#define	CMD_NUM12				12			/* 12							*/
//#define	CMD_NUM13				13			/* 13							*/
//#define	CMD_NUM14				14			/* 14							*/
//#define	CMD_NUM15				15			/* 15							*/
//#define	CMD_NUM16				16			/* 16							*/
//#define	CMD_NUM17				17			/* 17							*/
//#define	CMD_NUM18				18			/* 18							*/
//#define	CMD_NUM19				19			/* 19							*/
//#define	CMD_NUM20				20			/* 20							*/
//#define	CMD_NUM21				21			/* 21							*/
//#define	CMD_NUM22				22			/* 22							*/
//#define	CMD_NUM23				23			/* 23							*/
//#define	CMD_NUM24				24			/* 24							*/
//#define	CMD_NUM25				25			/* 25							*/
//#define	CMD_NUM26				26			/* 26							*/
//#define	CMD_NUM27				27			/* 27							*/
//#define	CMD_NUM28				28			/* 28							*/
//#define	CMD_NUM29				29			/* 29							*/
//#define	CMD_NUM30				30			/* 30							*/
//#define	CMD_NUM31				31			/* 31							*/
//#define	CMD_NUM32				32			/* 32							*/
//#define	CMD_NUM33				33			/* 33							*/
//#define	CMD_NUM36				36			/* 36							*/
//#define	CMD_NUM43				43			/* 43							*/
//#define	CMD_NUM44				44			/* 44							*/
//#define	CMD_NUM48				48			/* 48							*/
//#define	CMD_NUM60				60			/* 60							*/
//#define	CMD_NUM100				100			/* 100							*/
//#define	CMD_NUM128				128			/* 128							*/
//#define	CMD_NUM190				190			/* 190							*/
//#define	CMD_NUM200				200			/* 200							*/
//#define	CMD_NUM255				255			/* 255							*/
//#define	CMD_NUM256				256			/* 256							*/
//#define	CMD_NUM300				300			/* 300							*/
//#define	CMD_NUM1000				1000		/* 1000							*/
//#define	CMD_NUM3000				3000		/* 3000							*/
//#define	CMD_NUM10000			10000		/* 10000						*/
//#define	CMD_NUM12500			12500		/* 12500						*/
//
//#define CMD_BUFFER_SIZE_8192   8192  /* 8192  */

#define	CMD_NUM_0x0				0x0			/* 0x0							*/
#define	CMD_NUM_0xF				0xF			/* 0xF							*/
#define	CMD_NUM_0x00			0x00		/* 0x00							*/
#define	CMD_NUM_0xFF			0xFF		/* 0xFF							*/
#define	CMD_NUM_0x0000			0x0000		/* 0x0000						*/
#define	CMD_NUM_0xFFFF			0xFFFF		/* 0xFFFF						*/
#define	CMD_NUM_0x00000000		0x00000000	/* 0x00000000					*/
#define	CMD_NUM_0xFFFFFFFF		0xFFFFFFFF	/* 0xFFFFFFFF					*/
#define	CMD_NUM_0x108			0x108		/* 0x108						*/
#define	CMD_NUM_0x13C			0x13C		/* 0x13C						*/
#define	CMD_NUM_0x1A0			0x1A0		/* 0x1A0						*/
#define	CMD_NUM_0x2000			0x2000		/* 0x2000						*/
//
///* ======================================================================== */
///* CPRIリンク番号定義														*/
///* ======================================================================== */
//#define	CMD_CPRINO_MIN			1			/* CPRI#MIN						*/
//#define	CMD_CPRINO_01			1			/* CPRI# 1リンク番号			*/
//#define	CMD_CPRINO_02			2			/* CPRI# 2リンク番号			*/
//#define	CMD_CPRINO_03			3			/* CPRI# 3リンク番号			*/
//#define	CMD_CPRINO_04			4			/* CPRI# 4リンク番号			*/
//#define	CMD_CPRINO_05			5			/* CPRI# 5リンク番号			*/
//#define	CMD_CPRINO_06			6			/* CPRI# 6リンク番号			*/
//#define	CMD_CPRINO_MAX			6			/* CPRI#MAX						*/
//#define	CMD_CPRINO_NON			255			/* CPRI#なし					*/
//
///* ======================================================================== */
///* AMP番号定義																*/
///* ======================================================================== */
//#define	CMD_AMPNO_MIN			1			/* AMP#MIN						*/
//#define	CMD_AMPNO_1				1			/* AMP#1番号					*/
//#define	CMD_AMPNO_MAX			1			/* AMP#MAX						*/
//
///* ======================================================================== */
///* OF-TRXﾔ港															*/
///* ======================================================================== */
//#define	CMD_OFTRXNO_01			1			/* OF-TRX# 1					*/
//#define	CMD_OFTRXNO_02			2			/* OF-TRX# 2					*/
//#define	CMD_OFTRXNO_03			3			/* OF-TRX# 3					*/
//#define	CMD_OFTRXNO_04			4			/* OF-TRX# 4					*/
//#define	CMD_OFTRXNO_05			5			/* OF-TRX# 5					*/
//#define	CMD_OFTRXNO_06			6			/* OF-TRX# 6					*/
//#define	CMD_OFTRXNO_MAX			6			/* OFTRXﾅ大数					*/
//
///* ======================================================================== */
///* 外部装置番号定義															*/
///* ======================================================================== */
//#define	CMD_EXT_OARAIFN		1			/* OA-RA-INF					*/
//#define	CMD_EXT_TILTINF			2			/* TILT-INF						*/
//#define	CMD_EXT_KINDMAX			2			/* 外部装置種別最大数			*/
//
///* ======================================================================== */
///* スロット番号定義															*/
///* ======================================================================== */
#define	CMD_SLTNO_MIN			1			/* スロット#MIN					*/
//#define	CMD_SLTNO_01			1			/* スロット# 1番号				*/
//#define	CMD_SLTNO_02			2			/* スロット# 2番号				*/
//#define	CMD_SLTNO_03			3			/* スロット# 3番号				*/
//#define	CMD_SLTNO_04			4			/* スロット# 4番号				*/
//#define	CMD_SLTNO_05			5			/* スロット# 5番号				*/
//#define	CMD_SLTNO_06			6			/* スロット# 6番号				*/
//#define	CMD_SLTNO_07			7			/* スロット# 7番号				*/
//#define	CMD_SLTNO_08			8			/* スロット# 8番号				*/
//#define	CMD_SLTNO_09			9			/* スロット# 9番号				*/
//#define	CMD_SLTNO_10			10			/* スロット#10番号				*/
//#define	CMD_SLTNO_11			11			/* スロット#11番号				*/
//#define	CMD_SLTNO_12			12			/* スロット#12番号				*/
//#define	CMD_SLTNO_13			13			/* スロット#13番号				*/
//#define	CMD_SLTNO_14			14			/* スロット#14番号				*/
//#define	CMD_SLTNO_15			15			/* スロット#15番号				*/
//#define	CMD_SLTNO_16			16			/* スロット#16番号				*/
//#define	CMD_SLTNO_17			17			/* スロット#17番号				*/
//#define	CMD_SLTNO_18			18			/* スロット#18番号				*/
//#define	CMD_SLTNO_19			19			/* スロット#19番号				*/
//#define	CMD_SLTNO_20			20			/* スロット#20番号				*/
//#define	CMD_SLTNO_21			21			/* スロット#21番号				*/
//#define	CMD_SLTNO_22			22			/* スロット#22番号				*/
//#define	CMD_SLTNO_23			23			/* スロット#23番号				*/
//#define	CMD_SLTNO_24			24			/* スロット#24番号				*/
//#define	CMD_SLTNO_25			25			/* スロット#25番号				*/
//#define	CMD_SLTNO_26			26			/* スロット#26番号				*/
//#define	CMD_SLTNO_27			27			/* スロット#27番号				*/
//#define	CMD_SLTNO_28			28			/* スロット#28番号				*/
//#define	CMD_SLTNO_29			29			/* スロット#29番号				*/
//#define	CMD_SLTNO_30			30			/* スロット#30番号				*/
//#define	CMD_SLTNO_31			31			/* スロット#31番号				*/
//#define	CMD_SLTNO_32			32			/* スロット#32番号				*/
//#define	CMD_SLTNO_33			33			/* スロット#33番号				*/
//#define	CMD_SLTNO_34			34			/* スロット#34番号				*/
//#define	CMD_SLTNO_35			35			/* スロット#35番号				*/
//#define	CMD_SLTNO_36			36			/* スロット#36番号				*/
//#define	CMD_SLTNO_37			37			/* スロット#37番号				*/
//#define	CMD_SLTNO_38			38			/* スロット#38番号				*/
//#define	CMD_SLTNO_39			39			/* スロット#39番号				*/
//#define	CMD_SLTNO_40			40			/* スロット#40番号				*/
//#define	CMD_SLTNO_41			41			/* スロット#41番号				*/
//#define	CMD_SLTNO_42			42			/* スロット#42番号				*/
//#define	CMD_SLTNO_43			43			/* スロット#43番号				*/
//#define	CMD_SLTNO_44			44			/* スロット#44番号				*/
//#define	CMD_SLTNO_45			45			/* スロット#45番号				*/
//#define	CMD_SLTNO_46			46			/* スロット#46番号				*/
//#define	CMD_SLTNO_47			47			/* スロット#47番号				*/
//#define	CMD_SLTNO_48			48			/* スロット#48番号				*/
//#define	CMD_SLTNO_49			49			/* スロット#49番号				*/
//#define	CMD_SLTNO_50			50			/* スロット#50番号				*/
//#define	CMD_SLTNO_51			51			/* スロット#51番号				*/
//#define	CMD_SLTNO_52			52			/* スロット#52番号				*/
//#define	CMD_SLTNO_53			53			/* スロット#53番号				*/
//#define	CMD_SLTNO_54			54			/* スロット#54番号				*/
//#define	CMD_SLTNO_55			55			/* スロット#55番号				*/
//#define	CMD_SLTNO_56			56			/* スロット#56番号				*/
//#define	CMD_SLTNO_MAX			56			/* スロット#MAX					*/
//
//#if defined(OPT_CPRI_L2HARD)
//#define	CMD_SLOTNUM4			4			/*	スロット番号2(LTE BB/TRX-INF)	*/
//#define	CMD_SLOTNUM5			5			/*	スロット番号5(HWY/CMUX/CLK)	*/
//#endif
//
///* ======================================================================== */
///* スレッド間メッセージ名													*/
///* ======================================================================== */
//#define	CMD_TSKIF_THRSUPCMP_NTC		0x81010005	/*!< スレッド起動完了通知	*/
//#define	CMD_TSKIF_THRINI_REQ		0x81020000	/*!< スレッド初期化要求		*/
//#define	CMD_TSKIF_THRINI_RSP		0x81020001	/*!< スレッド初期化応答		*/
//#define	CMD_TSKIF_ALLTHRINICMP_NTC	0x81030005	/*!< 全スレッド初期化完了通知 */
//#define	CMD_TSKIF_ALLPRCINICMP_NTC	0x81040005	/*!< 全プロセス初期化完了通知 */
//#define	CMD_TSKIF_TIMOUTNTC			0x82010005	/*!< タイムアウト発生通知 */
//#define	CMD_TSKIF_ESTREQ			0x83010000	/*!< データリンク設定要求 */
//#define	CMD_TSKIF_ESTCNF			0x83010001	/*!< データリンク設定確認 */
//#define	CMD_TSKIF_ESTIND			0x83010005	/*!< データリンク設定表示 */
//#define	CMD_TSKIF_RELREQ			0x83020000	/*!< データリンク解放要求 */
//#define	CMD_TSKIF_RELCNF			0x83020001	/*!< データリンク解放確認 */
//#define	CMD_TSKIF_RELIND			0x83020005	/*!< データリンク解放表示 */
//#define	CMD_TSKIF_ESTINGIND			0x83030005	/*!< データリンク設定中表示 */
//#define	CMD_TSKIF_CPRIRCVNTC		0x83040005	/*!< CPRI信号受信通知 */
#define	CMD_TSKIF_CPRISNDIND		0x83050004	/*!< CPRI信号送信指示 */
//#define	CMD_TSKIF_L2BSY				0x83060005	/*!< 自受信ビジー発生通知 */
//#define	CMD_TSKIF_L2BSYCLR			0x83070005	/*!< 自受信ビジー解除通知 */
//#define	CMD_TSKIF_L2DEACTIND		0x83100004	/*!< L2 DEACT指示 */
//#define	CMD_TSKIF_L2STPRIND			0x83110004	/*!< L2 STOP指示 */
//#define	CMD_TSKIF_L1DATTXIND		0x83120004	/*!< L1データ送信指示 */
//#define	CMD_TSKIF_L1DATRCVNTC		0x83130005	/*!< L1データ受信通知 */
//#define	CMD_TSKIF_L1DATCNF			0x83140005	/*!< L1データ送信完了通知 */
//#define	CMD_TSKIF_TXABNMLNTC		0x83150005	/*!< TX送信異常通知 */
//#define	CMD_TSKIF_CPRSTACHGNTC		0x84030005	/*!< CPRIリンク状態変化通知 */
//#define	CMD_TSKIF_CPRDSCNTC			0x84040005	/*!< CPRIリンク断通知 */
//#define	CMD_TSKIF_ACTSETREQ			0x84050000	/*!< Active設定要求 */
//#define	CMD_TSKIF_ACTSETRSP			0x84050001	/*!< Active設定応答 */
//#define	CMD_TSKIF_ACTCHGIND			0x84060004	/*!< Active切替指示 */
//#define	CMD_TSKIF_ACTCHGRLTNTC		0x84070005	/*!< Active切替結果通知 */
//#define	CMD_TSKIF_NEGNTC			0x84080005	/*!< Negative通知 */
//#define	CMD_TSKIF_2NDRST_REQ		0x85010000	/*!< 二次リセット要求 */
//#define	CMD_TSKIF_2NDRST_RSP		0x85010001	/*!< 二次リセット応答 */
//#define	CMD_TSKIF_RECMPSTPNTC		0x85020005	/*!< 強制停止通知 */
//#define	CMD_TSKIF_RESRTCMPNTC		0x85030005	/*!< RE起動完了通知 */
//#define	CMD_TSKIF_REALLRSTNTC		0x85040004	/*!< 全RE強制停止指示 */
#define	CMD_TSKIF_2NDRSTPRCIND		0x85050004	/*!< 二次リセットプロシージャ実行指示 */
//#define	CMD_TSKIF_RETIMSETPRCIND	0x85060004	/*!< RE時刻設定プロシージャ実行指示 */
//#define	CMD_TSKIF_REPRMPRCIND		0x85070004	/*!< REシステムパラメータ更新プロシージャ実行指示 */
//#define	CMD_TSKIF_REFILPRCIND		0x85080004	/*!< REファイル更新プロシージャ実行指示 */
//#define	CMD_TSKIF_TOFVALPRCIND		0x85090004	/*!< Toffset値報告プロシージャ実行指示 */
#define	CMD_TSKIF_ACTSETPRCIND		0x850A0004	/*!< Active設定プロシージャ実行指示 */
//#define	CMD_TSKIF_REEQPINFPRCIND	0x850B0004	/*!< RE装置構成情報報告プロシージャ実行指示 */
//#define	CMD_TSKIF_RESLTINFPRCIND	0x850C0004	/*!< REスロット情報報告プロシージャ実行指示 */
//#define	CMD_TSKIF_RESTSPRCIND		0x850D0004	/*!< RE状態報告確プロシージャ実行指示 */
//#define	CMD_TSKIF_REPRTSTSPRCIND	0x850E0004	/*!< RE PORT状態報告プロシージャ実行指示 */
//#define	CMD_TSKIF_RECRDSSCPRCIND	0x850F0004	/*!< REカード状態報告(セクタ部)プロシージャ実行指示 */
//#define	CMD_TSKIF_RECRDSCMPRCIND	0x85100004	/*!< REカード状態報告(共通部)プロシージャ実行指示 */
//#define	CMD_TSKIF_RECRRSTSPRCIND	0x85110004	/*!< REキャリア状態報告プロシージャ実行指示 */
//#define	CMD_TSKIF_MTSETFPRCIND		0x85120004	/*!< MTアドレス設定(FLD-MT)プロシージャ実行指示 */
#define	CMD_TSKIF_MTSETSPRCIND		0x85130004	/*!< MTアドレス設定(SV-MT)プロシージャ実行指示 */
//#define	CMD_TSKIF_RESCMPPRCIND		0x85140004	/*!< RE起動完了プロシージャ実行指示 */
//#define	CMD_TSKIF_RERSTPRCIND		0x85150004	/*!< REリセットプロシージャ実行指示 */
//#define	CMD_TSKIF_REFRCRSTPRCIND	0x85160004	/*!< RE強制リセットプロシージャ実行指示 */
//#define	CMD_TSKIF_FRCSTPPRCIND		0x85170004	/*!< 強制停止プロシージャ実行指示 */
//#define	CMD_TSKIF_TXSAPRCIND		0x85180004	/*!< TRX設定(自律)プロシージャ実行指示 */
//#define	CMD_TSKIF_TXRAPRCIND		0x85190004	/*!< TRX解放(自律)プロシージャ実行指示 */
//#define	CMD_TSKIF_NMCAPRCIND		0x851A0004	/*!< 無変調キャリアON/OFF指定(自律)プロシージャ実行指示 */
//#define	CMD_TSKIF_REFRCRSTSETPRCIND	0x851B0004	/*!< RE強制リセット設置プロシージャ実行指示 */

#define	CMD_TSKIF_2NDRSTPRCIND_3G		0x85050005	/*!< 二次リセットプロシージャ実行指示 */
#define	CMD_TSKIF_RETIMSETPRCIND_3G		0x85060005	/*!< RE時刻設定プロシージャ実行指示 */
#define	CMD_TSKIF_REPRMPRCIND_3G		0x85070005	/*!< REシステムパラメータ更新プロシージャ実行指示 */
#define	CMD_TSKIF_REFILPRCIND_3G		0x85080005	/*!< REファイル更新プロシージャ実行指示 */
#define	CMD_TSKIF_TOFVALPRCIND_3G		0x85090005	/*!< Toffset値報告プロシージャ実行指示 */
#define	CMD_TSKIF_ACTSETPRCIND_3G		0x850A0005	/*!< Active設定プロシージャ実行指示 */
#define	CMD_TSKIF_REEQPINFPRCIND_3G		0x850B0005	/*!< RE装置構成情報報告プロシージャ実行指示 */
#define	CMD_TSKIF_RESLTINFPRCIND_3G		0x850C0005	/*!< REスロット情報報告プロシージャ実行指示 */
#define	CMD_TSKIF_RESTSPRCIND_3G		0x850D0005	/*!< RE状態報告確プロシージャ実行指示 */
#define	CMD_TSKIF_REPRTSTSPRCIND_3G		0x850E0005	/*!< RE PORT状態報告プロシージャ実行指示 */
#define	CMD_TSKIF_RECRDSSCPRCIND_3G		0x850F0005	/*!< REカード状態報告(セクタ部)プロシージャ実行指示 */
#define	CMD_TSKIF_RECRDSCMPRCIND_3G		0x85100005	/*!< REカード状態報告(共通部)プロシージャ実行指示 */
#define	CMD_TSKIF_RECRRSTSPRCIND_3G		0x85110005	/*!< REキャリア状態報告プロシージャ実行指示 */
#define	CMD_TSKIF_MTSETFPRCIND_3G		0x85120005	/*!< MTアドレス設定(FLD-MT)プロシージャ実行指示 */
#define	CMD_TSKIF_MTSETSPRCIND_3G		0x85130005	/*!< MTアドレス設定(SV-MT)プロシージャ実行指示 */
#define	CMD_TSKIF_RESCMPPRCIND_3G		0x85140005	/*!< RE起動完了プロシージャ実行指示 */
#define	CMD_TSKIF_RERSTPRCIND_3G		0x85150005	/*!< REリセットプロシージャ実行指示 */
#define	CMD_TSKIF_REFRCRSTPRCIND_3G		0x85160005	/*!< RE強制リセットプロシージャ実行指示 */
#define	CMD_TSKIF_FRCSTPPRCIND_3G		0x85170005	/*!< 強制停止プロシージャ実行指示 */
#define	CMD_TSKIF_TXSAPRCIND_3G			0x85180005	/*!< TRX設定(自律)プロシージャ実行指示 */
#define	CMD_TSKIF_TXRAPRCIND_3G			0x85190005	/*!< TRX解放(自律)プロシージャ実行指示 */
#define	CMD_TSKIF_NMCAPRCIND_3G			0x851A0005	/*!< 無変調キャリアON/OFF指定(自律)プロシージャ実行指示 */
#define	CMD_TSKIF_REFRCRSTSETPRCIND_3G	0x851B0005	/*!< RE強制リセット設置プロシージャ実行指示 */
#define	CMD_TSKIF_RESYSNEWREQ		0x86010000	/*!< REシステムパラメータ更新要求 */
#define	CMD_TSKIF_RESYSNEWRES		0x86010001	/*!< REシステムパラメータ更新応答 */
//#define	CMD_TSKIF_REFILNEWREQ		0x86020000	/*!< REファイル更新要求 */
//#define	CMD_TSKIF_REFILNEWRES		0x86020001	/*!< REファイル更新応答 */
//#define CMD_TSKIF_TRAFILDLMREQ     	0x86030000	/*!< TRA file download request to OAM */
//#define CMD_TSKIF_TRAFILDLMRES    	0x86030001	/*!< TRA file download response from OAM */
//#define	CMD_TSKIF_RENEWSTPNTC		0x86040004	/*!< RE更新停止指示 */
//#define	CMD_TSKIF_REDLPERMITREQ		0x86050000	/*!< ファイルデータ送信許可要求 */
//#define	CMD_TSKIF_REDLPERMITRSP		0x86050001	/*!< ファイルデータ送信許可応答 */
//#define CMD_TSKIF_CPRIMTSTARTIND	0x87010004	/*!< CPRI-MT運用開始指示			*/
//#define CMD_TSKIF_CPRIMTSTOPIND		0x87020004	/*!< CPRI-MT運用停止指示			*/
//#define CMD_TSKIF_CPRIDBGSTARTREQ	0x87030000	/*!< CPRIデバッグ機能開始要求		*/
//#define CMD_TSKIF_CPRIDBGSTARTRES	0x87030001	/*!< CPRIデバッグ機能開始応答		*/
//#define CMD_TSKIF_CPRIDBGSTOPREQ	0x87040000	/*!< CPRIデバッグ機能停止要求		*/
//#define CMD_TSKIF_CPRIDBGSTOPRES	0x87040001	/*!< CPRIデバッグ機能停止応答		*/
//#define CMD_TSKIF_CPRIMONISTARTIND	0x87050004	/*!< CPRI運用状態監視開始指示		*/
//#define CMD_TSKIF_CPRIMONISTOPIND	0x87060004	/*!< CPRI運用状態監視停止指示		*/
//#define CMD_TSKIF_L1MONDATRCVNTC	0x87070005	/*!< L1データ受信通知(運用状態監視) */
//#define CMD_TSKIF_L1MONDATSNDNTC	0x87080005	/*!< L1データ送信通知(運用状態監視) */
//#define CMD_TSKIF_L3DBGSTARTIND		0x87090005	/*!< L3デバッグ開始指示			*/
//#define CMD_TSKIF_L3DBGSTOPIND		0x870A0005	/*!< L3デバッグ停止指示			*/
//#define CMD_TSKIF_ACTCHGDISABLEREQ	0x870B0000	/*!< Active切替禁止要求			*/
//#define CMD_TSKIF_ACTCHGDISABLERES	0x870B0001	/*!< Active切替禁止応答			*/
//#define CMD_TSKIF_CPRILINKSTANTC	0x870C0005	/*!< CPRIリンクstate通知			*/
//#define CMD_TSKIF_TCPCONNNTC		0x870D0005	/*!< TCP接続通知					*/
//#define CMD_TSKIF_TCPRELNTC			0x870E0005	/*!< TCP解放通知					*/
//#define	CMD_TSKIF_OCMEVTRCV_NTC		0x89010005	/*!< OCMイベント受信通知 */
//#define	CMD_TSKIF_MTASTREQ			0x8A010000	/*!< MT Address Setting Request			*/
//#define	CMD_TSKIF_MTASTRES			0x8A010001	/*!< MT Address Setting Response		*/
//#define	CMD_TSKIF_MTDATAPERMITREQ	0x8A020000	/*!< MTデータ送信許可要求			*/
//#define	CMD_TSKIF_MTDATAPERMITRSP	0x8A020001	/*!< MTデータ送信許可応答			*/
//#define	CMD_TSKIF_NECHGSRTREQ		0x8C010000	/*!< N/E切替開始要求					*/
//#define	CMD_TSKIF_NECHGSRTRSP		0x8C010001	/*!< N/E切替開始応答					*/
//#define	CMD_TSKIF_NECHGCMPNTC		0x8C020005	/*!< N/E切替完了通知					*/
//#define	CMD_TSKIF_LBNECHGSRTREQ		0x8C030000	/*!< LAPB N/E切替開始要求				*/
//#define	CMD_TSKIF_LBNECHGSRTRSP		0x8C030001	/*!< LAPB N/E切替開始応答				*/
//#define	CMD_TSKIF_NEDATSRTNTC		0x8C040005	/*!< N/E間データ引継ぎ開始通知		*/
//#define	CMD_TSKIF_NEDATCMPNTC		0x8C050005	/*!< N/E間データ引継ぎ完了通知		*/
//#define	CMD_TSKIF_CPRRESTRNTC		0x8C060005	/*!< CPRI再起動通知					*/
//#define	CMD_TSKIF_CPRIRCVCMPNTC		0x8C070005	/*!< CPRIリンク回復完了通知			*/
//#define	CMD_TSKIF_SIGSTPCMPNTC      0x8C080005	/*!< Auto Signal Stop Complete Notice  */
//
///** ADD BY Wengzl Interface between CPRI MT and TRIF/MDIF  */
//#define CMD_CRDIF_CPRIDBGSTRREQ 	0x52000000  /*!< CPRIデバッグ機能開始要求 */
//#define CMD_CRDIF_CPRIDBGSTRRSP 	0x52000001  /*!< CPRIデバッグ機能開始応答 */
//#define CMD_CRDIF_CPRIDBGSTPREQ 	0x52010000  /*!< CPRIデバッグ機能停止要求 */
//#define CMD_CRDIF_CPRIDBGSTPRSP 	0x52010001  /*!< CPRIデバッグ機能停止応答 */
//#define CMD_CRDIF_DBGDATSNDIND 		0x52020004  /*!< CPRIデータ送信指示（デバッグ機能） */
//#define CMD_CRDIF_DBGDATRCVIND 		0x52030004  /*!< CPRIデータ受信指示（デバッグ機能） */
//#define CMD_CRDIF_CPRIMONSTRREQ 	0x52040000  /*!< CPRI運用状態監視開始要求 */
//#define CMD_CRDIF_CPRIMONSTRRSP 	0x52040001  /*!< CPRI運用状態監視開始応答 */
//#define CMD_CRDIF_CPRIMONSTPREQ 	0x52050000  /*!< CPRI運用状態監視停止要求 */
//#define CMD_CRDIF_CPRIMONSTPRSP 	0x52050001  /*!< CPRI運用状態監視停止応答 */
//#define CMD_CRDIF_MONDATSNDIND 		0x52060004  /*!< CPRIデータ送信指示（運用状態監視） */
//#define CMD_CRDIF_MONDATRCVIND 		0x52070004  /*!< CPRIデータ受信指示（運用状態監視） */
//#define CMD_CRDIF_CPRILNKSTANTCT 	0x52080004  /*!< CPRIリンクstate通知(TRIF) */
//#define CMD_CRDIF_CPRILNKSTANTCM 	0x52080004  /*!< CPRIリンクstate通知(MDIF) */
//#define CMD_CRDIF_ACTSWREQ	 		0x52090000  /*!< Active切替要求 */
//#define CMD_CRDIF_ACTSWRSP	 		0x52090001  /*!< Active切替応答*/
//#define CMD_CRDIF_ACTSWFBDREQ		0x520a0000  /*!< Active切替禁止要求 */
//#define CMD_CRDIF_ACTSWFBDRSP		0x520a0001  /*!< Active切替禁止応答 */
//#define CMD_CRDIF_HBREQ 			0x520b0000  /*!< Heart beat要求 */
//#define CMD_CRDIF_HBRSP 			0x520b0001  /*!< Heart beat応答 */
//
///* ======================================================================== */
///* CPRI信号種別定義 														*/
///* ======================================================================== */
//#define	CMD_CPRID_HCREQ			0x0111	/* ヘルスチェック要求				*/
//#define	CMD_CPRID_HCRES			0x0121	/* ヘルスチェック応答				*/
//#define	CMD_CPRID_RERSTREQ		0x0211	/* REリセット要求					*/
//#define	CMD_CPRID_RETIMSETREQ	0x0311	/* RE時刻設定要求					*/
//#define	CMD_CPRID_LINKSTSREQ	0x1111	/* RE共通部リンク確立要求			*/
//#define	CMD_CPRID_LINKSTSRES	0x1121	/* RE共通部リンク確立応答			*/
//#define	CMD_CPRID_FIRMFILEREQ	0x2111	/* ファームウェアファイル報告要求	*/
//#define	CMD_CPRID_FIRMFILERES	0x2121	/* ファームウェアファイル報告応答	*/
//#define	CMD_CPRID_FILEINFREQ	0x2211	/* ファイル情報報告通知				*/
//#define	CMD_CPRID_FILEINFRES	0x2221	/* ファイル情報報告応答				*/
//#define	CMD_CPRID_FILEDATSND	0x2301	/* ファイルデータ送信				*/
//#define	CMD_CPRID_FILSNDFINNTC	0x2311	/* ファイルデータ送信完了報告通知	*/
//#define	CMD_CPRID_FILSNDFINRES	0x2321	/* ファイルデータ送信完了報告応答	*/
//#define	CMD_CPRID_UFIRMFILEREQ	0x2411	/* 運用中ファームウェアファイル報告要求	*/
//#define	CMD_CPRID_UFIRMFILERES	0x2421	/* 運用中ファームウェアファイル報告応答	*/
//#define	CMD_CPRID_UFILEINFREQ	0x2511	/* 運用中ファイル情報報告通知			*/
//#define	CMD_CPRID_UFILEINFRES	0x2521	/* 運用中ファイル情報報告応答			*/
//#define	CMD_CPRID_UFILEDATSND	0x2601	/* 運用中ファイルデータ送信				*/
//#define	CMD_CPRID_UFILSNDFINNTC	0x2611	/* 運用中ファイルデータ送信完了報告通知	*/
//#define	CMD_CPRID_UFILSNDFINRES	0x2621	/* 運用中ファイルデータ送信完了報告応答	*/
//#define	CMD_CPRID_TRXSETREQ		0x4111	/* TRX設定要求						*/
//#define	CMD_CPRID_TRXSETRES		0x4121	/* TRX設定応答						*/
//#define	CMD_CPRID_TRXRELREQ		0x4211	/* TRX解放要求						*/
//#define	CMD_CPRID_TRXRELRES		0x4221	/* TRX解放応答						*/
//#define	CMD_CPRID_TOFFSETINFREQ	0x4311	/* Toffset値報告要求				*/
//#define	CMD_CPRID_TOFFSETINFRES	0x4321	/* Toffset値報告応答				*/
//#define	CMD_CPRID_CARONOFFREQ	0x4411	/* 無変調キャリアON/OFF指定要求		*/
//#define	CMD_CPRID_CARONOFFRES	0x4421	/* 無変調キャリアON/OFF指定応答		*/
//#define	CMD_CPRID_EQPSTRREQ		0x5111	/* RE装置構成情報報告要求			*/
//#define	CMD_CPRID_EQPSTRRES		0x5121	/* RE装置構成情報報告応答			*/
//#define	CMD_CPRID_SLOTINFREQ	0x5211	/* REスロット情報報告要求			*/
//#define	CMD_CPRID_SLOTINFRES	0x5221	/* REスロット情報報告応答			*/
//#define	CMD_CPRID_RESTSREQ		0x5311	/* RE状態報告要求					*/
//#define	CMD_CPRID_RESTSRES		0x5321	/* RE状態報告応答					*/
//#define	CMD_CPRID_CARDINFREQ	0x5411	/* REカード情報報告要求				*/
//#define	CMD_CPRID_CARDINFRES	0x5421	/* REカード情報報告応答				*/
//#define	CMD_CPRID_REVERREQ		0x5511	/* REバージョン報告要求				*/
//#define	CMD_CPRID_REVERRES		0x5521	/* REバージョン報告応答				*/
//#define	CMD_CPRID_RECARDCNTREQ	0x5611	/* REカード制御要求					*/
//#define	CMD_CPRID_RECARDCNTRES	0x5621	/* REカード制御応答					*/
//#define	CMD_CPRID_CARDSTSREQ	0x5711	/* REカード状態報告要求				*/
//#define	CMD_CPRID_CARDSTSRES	0x5721	/* REカード状態報告応答				*/
//#define	CMD_CPRID_CARLSTSREQ	0x5811	/* REキャリア状態報告要求			*/
//#define	CMD_CPRID_CARLSTSRES	0x5821	/* REキャリア状態報告応答			*/
//#define	CMD_CPRID_REPORTCNTREQ	0x5911	/* RE PORT制御要求					*/
//#define	CMD_CPRID_REPORTCNTRES	0x5921	/* RE PORT制御応答					*/
//#define	CMD_CPRID_REPORTSTSREQ	0x5A11	/* RE PORT状態報告要求				*/
//#define	CMD_CPRID_REPORTSTSRES	0x5A21	/* RE PORT状態報告応答				*/
//#define	CMD_CPRID_RECRDDIAREQ	0x5B11	/* REカード診断要求					*/
//#define	CMD_CPRID_RECRDDIARES	0x5B21	/* REカード診断報告					*/
//#define	CMD_CPRID_REOBSLOGREQ	0x5C11	/* RE障害ログ取得要求				*/
//#define	CMD_CPRID_REOBSLOGRES	0x5C21	/* RE障害ログ取得応答				*/
//#define	CMD_CPRID_REOBLOGSTPREQ	0x5D11	/* RE障害ログ取得中止要求			*/
//#define	CMD_CPRID_REOBLOGSTPRES	0x5D21	/* RE障害ログ取得中止応答			*/
//#define	CMD_CPRID_EXTDATSNDREQ	0x9111	/* 外部装置データ送信要求			*/
//#define	CMD_CPRID_EXTDATSNDRES	0x9121	/* 外部装置データ送信応答			*/
//#define	CMD_CPRID_EXTDATRCVREQ	0x9131	/* 外部装置データ受信要求			*/
//#define	CMD_CPRID_EXTDATRCVRES	0x9141	/* 外部装置データ受信応答			*/
//#define	CMD_CPRID_MTADDSETREQ	0x9211	/* MTアドレス設定要求				*/
//#define	CMD_CPRID_MTADDSETRES	0x9221	/* MTアドレス設定応答				*/
//#define	CMD_CPRID_FLDMTCNCREQ	0x9311	/* FLD-MT接続要求					*/
//#define	CMD_CPRID_FLDMTCNCRES	0x9321	/* FLD-MT接続応答					*/
//#define	CMD_CPRID_FLDCOMSNDREQ	0x9371	/* FLD-MTデータコマンド転送送信要求	*/
//#define	CMD_CPRID_FLDCOMSNDRES	0x9381	/* FLD-MTデータコマンド転送送信応答	*/
//#define	CMD_CPRID_FLDCOMRCVREQ	0x9391	/* FLD-MTデータコマンド転送受信要求	*/
//#define	CMD_CPRID_FLDCOMRCVRES	0x93A1	/* FLD-MTデータコマンド転送受信応答	*/
//#define	CMD_CPRID_FLDFILSNDREQ	0x93B1	/* FLD-MTデータファイル転送送信要求	*/
//#define	CMD_CPRID_FLDFILSNDRES	0x93C1	/* FLD-MTデータファイル転送送信応答	*/
//#define	CMD_CPRID_FLDFILRCVREQ	0x93D1	/* FLD-MTデータファイル転送受信要求	*/
//#define	CMD_CPRID_FLDFILRCVRES	0x93E1	/* FLD-MTデータファイル転送受信応答	*/
//#define	CMD_CPRID_FLMTCNCRELREQ	0x9331	/* FLD-MT接続解放要求				*/
//#define	CMD_CPRID_FLMTCNCRELRES	0x9341	/* FLD-MT接続解放応答				*/
//#define	CMD_CPRID_FLMTCMPRELREQ	0x9351	/* FLD-MT接続強制解放要求			*/
//#define	CMD_CPRID_FLMTCMPRELRES	0x9361	/* FLD-MT接続強制解放応答			*/
//#define	CMD_CPRID_SVMTCNTREQ	0x9411	/* SV-MT接続要求					*/
//#define	CMD_CPRID_SVMTCNTRES	0x9421	/* SV-MT接続応答					*/
//#define	CMD_CPRID_SVMTDATSNDREQ	0x9471	/* SV-MTデータ送信要求				*/
//#define	CMD_CPRID_SVMTDATSNDRES	0x9481	/* SV-MTデータ送信応答				*/
//#define	CMD_CPRID_SVMTDATRCVREQ	0x9491	/* SV-MTデータ受信要求				*/
//#define	CMD_CPRID_SVMTDATRCVRES	0x94A1	/* SV-MTデータ受信応答				*/
//#define	CMD_CPRID_SVMTCNTRELREQ	0x9431	/* SV-MT接続解放要求				*/
//#define	CMD_CPRID_SVMTCNTRELRES	0x9441	/* SV-MT接続解放応答				*/
//#define	CMD_CPRID_SVMTFORRELREQ	0x9451	/* SV-MT接続強制解放要求			*/
//#define	CMD_CPRID_SVMTFORRELRES	0x9461	/* SV-MT接続強制解放応答			*/
//#define	CMD_CPRID_MKMTDATSND	0x9501	/* メーカMTデータ送信				*/
//#define	CMD_CPRID_MKMTCNTREQ	0x9511	/* メーカMT接続要求					*/
//#define	CMD_CPRID_MKMTCNTRES	0x9521	/* メーカMT接続応答					*/
//#define	CMD_CPRID_MKMTCNTRELREQ	0x9531	/* メーカMT接続解放要求				*/
//#define	CMD_CPRID_MKMTCNTRELRES	0x9541	/* メーカMT接続解放応答				*/
//#define	CMD_CPRID_MKMTFORRELREQ	0x9551	/* メーカMT接続強制解放要求			*/
//#define	CMD_CPRID_MKMTFORRELRES	0x9561	/* メーカMT接続強制解放応答			*/
//
//#define	CMD_CPRID_MUXDUMMYFRM_OFF	0xFFF1	/* ダミーL3メッセージ(Log取得無)	*/
//#define	CMD_CPRID_MUXDUMMYFRM_ON	0xFFE1	/* ダミーL3メッセージ(Log取得有)	*/
//
///* ======================================================================== */
///* CPRI信号データ部レングス定義												*/
///* ======================================================================== */
//#define	CMD_CPRLN_HCREQ			22	 	/* ヘルスチェック要求				*/
//#define	CMD_CPRLN_HCRES					/* ヘルスチェック応答				*/
//#define	CMD_CPRLN_RERSTREQ		6		/* REリセット要求					*/
//#define	CMD_CPRLN_RETIMSETREQ	10		/* RE時刻設定要求					*/
//#define	CMD_CPRLN_LINKSTSREQ	24		/* RE共通部リンク確立要求			*/
//#define	CMD_CPRLN_LINKSTSRES			/* RE共通部リンク確立応答			*/
//#define	CMD_CPRLN_FIRMFILEREQ	2		/* ファームウェアファイル報告要求	*/
//#define	CMD_CPRLN_FIRMFILERES	10		/* ファームウェアファイル報告応答	*/
//#define	CMD_CPRLN_FILEINFREQ	28		/* ファイル情報報告通知				*/
//#define	CMD_CPRLN_FILEINFRES			/* ファイル情報報告応答				*/
//#define	CMD_CPRLN_FILEDATSND	10		/* ファイルデータ送信				*/
//#define	CMD_CPRLN_FILSNDFINNTC	6		/* ファイルデータ送信完了報告通知	*/
//#define	CMD_CPRLN_FILSNDFINRES			/* ファイルデータ送信完了報告応答	*/
//#define	CMD_CPRLN_TRXSETREQ		10		/* TRX設定要求						*/
//#define	CMD_CPRLN_TRXSETRES				/* TRX設定応答						*/
//#define	CMD_CPRLN_TRXRELREQ		4		/* TRX解放要求						*/
//#define	CMD_CPRLN_TRXRELRES				/* TRX解放応答						*/
//#define	CMD_CPRLN_TOFFSETINFREQ	  2		/* Toffset値報告要求				*/
//#define	CMD_CPRLN_TOFFSETINFRES			/* Toffset値報告応答				*/
//#define	CMD_CPRLN_CARONOFFREQ	 8		/* 無変調キャリアON/OFF指定要求		*/
//#define	CMD_CPRLN_CARONOFFRES			/* 無変調キャリアON/OFF指定応答		*/
//#define	CMD_CPRLN_EQPSTRREQ		  2		/* RE装置構成情報報告要求			*/
//#define	CMD_CPRLN_EQPSTRRES				/* RE装置構成情報報告応答			*/
//#define	CMD_CPRLN_SLOTINFREQ	  2		/* REスロット情報報告要求			*/
//#define	CMD_CPRLN_SLOTINFRES			/* REスロット情報報告応答			*/
//#define	CMD_CPRLN_RESTSREQ		  2		/* RE状態報告要求					*/
//#define	CMD_CPRLN_RESTSRES				/* RE状態報告応答					*/
//#define	CMD_CPRLN_CARDINFREQ			/* REカード情報報告要求				*/
//#define	CMD_CPRLN_CARDINFRES			/* REカード情報報告応答				*/
//#define	CMD_CPRLN_REVERREQ		  8		/* REバージョン報告要求				*/
//#define	CMD_CPRLN_REVERRES				/* REバージョン報告応答				*/
//#define	CMD_CPRLN_RECARDCNTREQ	  6		/* REカード制御要求					*/
//#define	CMD_CPRLN_RECARDCNTRES			/* REカード制御応答					*/
//#define	CMD_CPRLN_CARDSTSREQ	  4		/* REカード状態報告要求				*/
//#define	CMD_CPRLN_CARDSTSRES			/* REカード状態報告応答				*/
//#define	CMD_CPRLN_CARLSTSREQ	  2		/* REキャリア状態報告要求			*/
//#define	CMD_CPRLN_CARLSTSRES			/* REキャリア状態報告応答			*/
//#define	CMD_CPRLN_REPORTCNTREQ	  6		/* RE PORT制御要求					*/
//#define	CMD_CPRLN_REPORTCNTRES			/* RE PORT制御応答					*/
//#define	CMD_CPRLN_REPORTSTSREQ	  2		/* RE PORT状態報告要求				*/
//#define	CMD_CPRLN_REPORTSTSRES			/* RE PORT状態報告応答				*/
//#define	CMD_CPRLN_RECRDDIAREQ	  4		/* REカード診断要求					*/
//#define	CMD_CPRLN_RECRDDIARES			/* REカード診断報告					*/
//#define	CMD_CPRLN_REOBSLOGREQ	260		/* RE障害ログ取得要求				*/
//#define	CMD_CPRLN_REOBSLOGRES			/* RE障害ログ取得応答				*/
//#define	CMD_CPRLN_REOBLOGSTPREQ	2		/* RE障害ログ取得中止要求			*/
//#define	CMD_CPRLN_REOBLOGSTPRES			/* RE障害ログ取得中止応答			*/
//#define	CMD_CPRLN_EXTDATSNDREQ	 2054	/* 外部装置データ送信要求			*/
//#define	CMD_CPRLN_EXTDATSNDRES			/* 外部装置データ送信応答			*/
//#define	CMD_CPRLN_EXTDATRCVREQ			/* 外部装置データ受信要求			*/
//#define	CMD_CPRLN_EXTDATRCVRES	 6		/* 外部装置データ受信応答			*/
//#define	CMD_CPRLN_MTADDSETREQ	 18		/* MTアドレス設定要求				*/
//#define	CMD_CPRLN_MTADDSETRES			/* MTアドレス設定応答				*/
//#define	CMD_CPRLN_FLDMTCNCREQ			/* FLD-MT接続要求					*/
//#define	CMD_CPRLN_FLDMTCNCRES			/* FLD-MT接続応答					*/
//#define	CMD_CPRLN_FLDCOMSNDREQ			/* FLD-MTデータコマンド転送送信要求	*/
//#define	CMD_CPRLN_FLDCOMSNDRES			/* FLD-MTデータコマンド転送送信応答	*/
//#define	CMD_CPRLN_FLDCOMRCVREQ			/* FLD-MTデータコマンド転送受信要求	*/
//#define	CMD_CPRLN_FLDCOMRCVRES			/* FLD-MTデータコマンド転送受信応答	*/
//#define	CMD_CPRLN_FLDFILSNDREQ			/* FLD-MTデータファイル転送送信要求	*/
//#define	CMD_CPRLN_FLDFILSNDRES			/* FLD-MTデータファイル転送送信応答	*/
//#define	CMD_CPRLN_FLDFILRCVREQ			/* FLD-MTデータファイル転送受信要求	*/
//#define	CMD_CPRLN_FLDFILRCVRES			/* FLD-MTデータファイル転送受信応答	*/
//#define	CMD_CPRLN_FLMTCNCRELREQ			/* FLD-MT接続解放要求				*/
//#define	CMD_CPRLN_FLMTCNCRELRES			/* FLD-MT接続解放応答				*/
//#define	CMD_CPRLN_FLMTCMPRELREQ			/* FLD-MT接続強制解放要求			*/
//#define	CMD_CPRLN_FLMTCMPRELRES			/* FLD-MT接続強制解放応答			*/
//#define	CMD_CPRLN_SVMTCNTREQ			/* SV-MT接続要求					*/
//#define	CMD_CPRLN_SVMTCNTRES			/* SV-MT接続応答					*/
//#define	CMD_CPRLN_SVMTDATSNDREQ			/* SV-MTデータ送信要求				*/
//#define	CMD_CPRLN_SVMTDATSNDRES			/* SV-MTデータ送信応答				*/
//#define	CMD_CPRLN_SVMTDATRCVREQ			/* SV-MTデータ受信要求				*/
//#define	CMD_CPRLN_SVMTDATRCVRES			/* SV-MTデータ受信応答				*/
//#define	CMD_CPRLN_SVMTCNTRELREQ			/* SV-MT接続解放要求				*/
//#define	CMD_CPRLN_SVMTCNTRELRES			/* SV-MT接続解放応答				*/
//#define	CMD_CPRLN_SVMTFORRELREQ			/* SV-MT接続強制解放要求			*/
//#define	CMD_CPRLN_SVMTFORRELRES			/* SV-MT接続強制解放応答			*/
//#define	CMD_CPRLN_MKMTDATSND			/* メーカMTデータ送信				*/
//#define	CMD_CPRLN_MKMTCNTREQ			/* メーカMT接続要求					*/
//#define	CMD_CPRLN_MKMTCNTRES			/* メーカMT接続応答					*/
//#define	CMD_CPRLN_MKMTCNTRELREQ			/* メーカMT接続解放要求				*/
//#define	CMD_CPRLN_MKMTCNTRELRES			/* メーカMT接続解放応答				*/
//#define	CMD_CPRLN_MKMTFORRELREQ			/* メーカMT接続強制解放要求			*/
//#define	CMD_CPRLN_MKMTFORRELRES			/* メーカMT接続強制解放応答			*/
//#define	CMD_CPRLN_UFIRMFILEREQ	2		/* 運用中ファームウェアファイル報告要求	*/
//#define	CMD_CPRLN_UFIRMFILERES	14		/* 運用中ファームウェアファイル報告応答	*/
//#define	CMD_CPRLN_UFILEINFREQ	28		/* 運用中ファイル情報報告通知				*/
//#define	CMD_CPRLN_UFILEINFRES			/* 運用中ファイル情報報告応答				*/
//#define	CMD_CPRLN_UFILEDATSND	10		/* 運用中ファイルデータ送信				*/
//#define	CMD_CPRLN_UFILSNDFINNTC	6		/* 運用中ファイルデータ送信完了報告通知	*/
//#define	CMD_CPRLN_UFILSNDFINRES			/* 運用中ファイルデータ送信完了報告応答	*/
//
//
///************L2 buffer usage flag*************/
//#define CMD_L2_TX_SU			1		/* buffer usage for sending S and U frame*/
//#define CMD_L2_TX_I				2		/* buffer usage for sending I frame	*/
//#define CMD_L2_RX				3		/* buffer usage for receiving frame	*/
//
///* ======================================================================== */
///* メーカMT mode定義				 										*/
///* ======================================================================== */
//#define CMD_MKMTMODE_MNI			0	/* monitor							*/
//#define CMD_MKMTMODE_SIM			1	/* simulator						*/
//
///* ======================================================================== */
///* CPRI simulator mode定義			 										*/
///* ======================================================================== */
//#define CMD_SIMMODE_COMMON			1	/* common mode						*/
//#define CMD_SIMMODE_L3TEST			2	/* L3 test mode						*/
//
///* ======================================================================== */
///* CPRI simulator link specify range		 								*/
///* ======================================================================== */
//#define CMD_SIM_CPRIRANGE_SIGNLE	1	/* signle simulator link			*/
//#define CMD_SIM_CPRIRANGE_ALL		2	/* all simulator links				*/
//
///* ======================================================================== */
///* 1. RE共通部active / negative定義 for MDIF RE共通部情報共通管理テーブル	*/
///* 2. 設定類別定義	for MDIF task IF RE共通部リンク再確立通知				*/
///* 3. return code定義  for common function m_cm_ActiveChk					*/
///* 4. return code定義  for common function t_cm_ActiveChk					*/
///* ======================================================================== */
//#define CMD_RECLS_ACTSET			0	/* active設定						*/
//#define CMD_RECLS_NEGSET			1	/* negative設定						*/
//#define CMD_RECLS_ALLACTSET			2	/* 無条件active設定					*/
//
///* ======================================================================== */
///*	MT type 定義															*/
///* ======================================================================== */
//#define CMD_MTTYPE_FLD				0	/* FLD MT							*/
//#define CMD_MTTYPE_SV				1	/* SV MT							*/
//#define CMD_MTTYPE_MK				2	/* Maker MT							*/
//
///* ======================================================================== */
///*	TCP link type 定義			 											*/
///* ======================================================================== */
//#define CMD_LINKTYPE_FLDCMD		0	/*TCP link for FLD command data transfer*/
//#define CMD_LINKTYPE_FLDFILE	1	/*TCP link for FLD file data transfer	*/
//#define CMD_LINKTYPE_SV			2	/*TCP link for SV data transfer			*/
//#define CMD_LINKTYPE_MKSIMU		3	/*TCP link for Maker simulator data transfer*/
//#define CMD_LINKTYPE_MKMON		4	/*TCP link for Maker monitor data transfer*/
//#define CMD_LINKTYPE_OPT		5	/*TCP link for OPT data transfer*/

//#define CMD_LINK_NUM_T			6

//#define CMD_LINK_NUM_PC	    	12 /* CPRI Link num, only used between PC and TRIF */
//
//#define CMD_LINK_NUM_M			1	/* Amount of CPRI links in each MDIF card*/
//
//#define CMD_BANK_NUM			16	/* Amount of banks of each link			*/
//
///* ======================================================================== */
///* MAX CPRI number define in one AMP										*/
///* ======================================================================== */
//#define CMD_MX_AMPCPRI			6		/* MAX CPRI number in one AMP		*/
//
///* ======================================================================== */
///*						  		メーカ固有情報数							*/
///* ======================================================================== */
//#define	CMD_MAKER_NUM			32			/*	メーカ固有情報数			*/
//#define	CMD_FAN_NUM				16			/*	FAN情報数					*/
//#define	CMD_ALM_NUM				16			/*	ALM情報数					*/
//#define	CMD_ERR_NUM				16			/*	ERR情報数					*/
//
///****************************************************************************/
///*																			*/
///*								ALM要因定義									*/
///*																			*/
///****************************************************************************/
///*	共通関数要因１（システムコール系）	*/
//#define	CMD_ALMCD_BUFGET		0x0022		/*	バッファ取得異常			*/
//#define	CMD_ALMCD_BUFREL		0x0023		/*	バッファ解放異常			*/
//#define	CMD_ALMCD_TIMSTAT		0x0024		/*	タイマ起動異常				*/
//#define	CMD_ALMCD_TIMSTOP		0x0025		/*	タイマ停止異常				*/
//#define	CMD_ALMCD_SEMGET		0x0026		/*	セマフォ取得異常			*/
//#define	CMD_ALMCD_SEMREL		0x0027		/*	セマフォ返却異常			*/
//#define	CMD_ALMCD_CMNFUNC1		0x002F		/*	その他異常					*/
///*added*/
//#define CMD_ALMCD_DATA			0x0036		/*  データ異常 					*/
//#define CMD_ALMCD_CMNFUNC		0x0031		/*  状態異常 					*/
//
//#if defined(OPT_CPRI_L2HARD)
//#define CMD_ALMCD_TXABNL		58			/*  CPRI#1 L3データ送信異常(0x003A)	*/
//#define CMD_ALMCD_RXABNL		59			/*  CPRI#1 L3データ受信異常(0x003B)	*/
//#define CMD_ALMCD_TXABNL_C2		66			/*  CPRI#2 L3データ送信異常(0x0042)	*//*	未使用(CPRI#1 ALMCODE + 8)にて算出するため	*/
//#define CMD_ALMCD_RXABNL_C2		67			/*  CPRI#2 L3データ受信異常(0x0043)	*//*	未使用(CPRI#1 ALMCODE + 8)にて算出するため	*/
//#endif

/****************************************************************************/
/*				LOG trace level definition									*/
/****************************************************************************/
#define F_COMF_CMM_FFUNCTRCLOG(logLevel, b, c)		;	//性能改善 20151101 enoki  BPF_COM_LOG_ASSERT(logLevel, "")
#define F_COMF_CMM_FINVDLOG(logLevel,msg,c,d)		BPF_COM_LOG_ASSERT(logLevel, msg)

#define D_TCM_TRCLOGLV_OTH		D_RRH_LOG_AST_LV_ENTER	/*!< Trace log for other functions
							(except common function and event first handle function) */
#define D_TCM_INVLOGLV_NOTICE		D_RRH_LOG_AST_LV_INFO		/*!< Invalid log for fail-safe and RASIS function */
#define D_TCM_INVLOGLV_WARN1		D_RRH_LOG_AST_LV_WARNING	/*!< Invalid log for timeout, NG response, ALM switch/drive out etc */
#define D_TCM_INVLOGLV_WARN2		D_RRH_LOG_AST_LV_WARNING	/*!< Invalid log for retry NG,end code NG etc */
#define D_TCM_INVLOGLV_ALERT		D_RRH_LOG_AST_LV_ERROR		/*!< Invalid log for Hard ALM, OS resource NG, logical NG etc */

///****************************************************************************/
///*				LOG trace data length definition							*/
///****************************************************************************/
//
//#define D_TCM_TRCLOG_MAXDATLEN			32	/*!< function trace log data max length for 1 parameter */
//#define D_TCM_INVLOG_MAXDATLEN			48	/*!< Invalid log data max length for userdata2 parameter */
//
///****************************************************************************/
///*				logout parameter defination							*/
///****************************************************************************/
///* LOG type */
//#define D_TCM_LOGOUT_TYPE_BOTH		0		/*!< both resource and status */
//#define D_TCM_LOGOUT_TYPE_RES		1		/*!< resource */
//#define D_TCM_LOGOUT_TYPE_STA		2		/*!< status */
//
///* thread name */
//#define D_TCM_LOGOUT_THRNAME_ALL	0		/*!< ALL thread */
///* 1~11 is TRM thread ~ MTR thread, 12 is TCM */
//#define D_TCM_LOGOUT_THRNAME_MAX	13		/*!< MAX thread */
//
///* log kind for each thread  */
#define D_TCM_LOGOUT_KIND_ALL			0		/*!< ALL log */	
#define D_TCM_LOGOUT_KIND_RUNHIS		1		/*!< runhistory */
#define D_TCM_LOGOUT_KIND_REGHIS		2		/*!< register history */
#define D_TCM_LOGOUT_KIND_STN			3		/*!< status */
#define D_TCM_LOGOUT_KIND_TIM			4		/*!< Timer */
#define D_TCM_LOGOUT_KIND_DPL			5		/*!< dump list */
#define D_TCM_LOGOUT_KIND_DMP			6		/*!< dump table */
//#define D_TCM_LOGOUT_KIND_VER			7		/*!< version */
//#define D_TCM_LOGOUT_KIND_CPR			8		/*!< CPRI link status */
//#define D_TCM_LOGOUT_KIND_SYSPRM		9		/*!< system parameter */
//#define D_TCM_LOGOUT_KIND_TID			10		/*!< thread ID */
//#define D_TCM_LOGOUT_KIND_BUF			11		/*!< remain buffer number */
//#define D_TCM_LOGOUT_KIND_ITM			12		/*!< holding message number(IT) */
//#define D_TCM_LOGOUT_KIND_IPM			13		/*!< holding message number(IP) */
//#define D_TCM_LOGOUT_KIND_L1ERR			14		/*!< L1 Data send/recv Log */
//#define D_TCM_LOGOUT_KIND_MESSTA		15		/*!< Message Statistics Log */
//#define D_TCM_LOGOUT_KIND_RUNHIS_LPBERR 16		/*!< LPB 異常ログ */
#define D_TCM_LOGOUT_KIND_RUNHIS_ALL	17		/*!< runhistory ALL */
#define D_TCM_LOGOUT_KIND_FBSTSDMP		18		/*!< 機能部状態/故障状態*/
//
#define D_TCM_REALMWATCH_DBGON			20		/*!< RE ALM状態監視時間変更(10分) */
#define D_TCM_REALMWATCH_DBGOFF			21		/*!< RE ALM状態監視時間変更無 */
#define D_TCM_REALMATIME_DSP			22		/*!< RE ALM状態監視時刻表示 */
#define D_TCM_CMUX_REG_DSP				23
//
//#if defined(OPT_CPRI_L2HARD)
//#define	D_TCM_SYSPARATBL_NUM			5		/*!< syspara setting address number */
//#endif
//
///* dma dump thread for txs/rxs thread */
//#define D_TCM_DMAOUT_ALL		0		/*!< ALL */	
//
///* dma dump kind for txs/rxs thread */
//#define D_TCM_DMATOK_ALL		0		/*!< ALL */	
//#define D_TCM_DMATOK_DES		1		/*!< Decscriptor */
//#define D_TCM_DMATOK_BUF		2		/*!< Buffer */
//#define D_TCM_DMATOK_STS		3		/*!< Buffer status */
//#define D_TCM_DMATOK_LST		4		/*!< Token list */
//
//#if defined(OPT_CPRI_L2HARD)
//#define CMD_DMA1_SHIFTNUM		24		/*!< DMA割込み1 bit shift value	*/
//#define CMD_DMA2_SHIFTNUM		23		/*!< DMA割込み2 bit shift value	*/
//#define CMD_DMA3_SHIFTNUM		22		/*!< DMA割込み3 bit shift value	*/
//#define CMD_DMA4_SHIFTNUM		21		/*!< DMA割込み4 bit shift value	*/
//#define CMD_DMA5_SHIFTNUM		20		/*!< DMA割込み5 bit shift value	*/
//#define CMD_DMA6_SHIFTNUM		19		/*!< DMA割込み6 bit shift value	*/
//#define CMD_DMA7_SHIFTNUM		18		/*!< DMA割込み7 bit shift value	*/
//#define CMD_DMA8_SHIFTNUM		17		/*!< DMA割込み8 bit shift value	*/
//
//#define CMD_DMAKIND_RCV_NUM		3		/*!< DMA割込み(受信系)種別数	*/
//#define CMD_DMAKIND_LTE			0		/*!< DMA割込み種別(LTE)			*/
//#define CMD_DMAKIND_CPRSND		1		/*!< DMA割込み種別(LTE)			*/
//#define CMD_DMAKIND_CPRRCV		2		/*!< DMA割込み種別(LTE)			*/
//
//#define CMD_DMA_NUM				6		/*!< DMAC数						*/
//#define CMD_DMALINK_NUM			2		/*!< DMA CPRIリンク数			*/
//
//#define CMD_CPRITR_DWM			1		/*!<	方向(REC->RE)	*/
//#define CMD_CPRITR_UP			2		/*!<	方向(RE->REC)	*/
//
//#define CMD_MSK_DMACPRCHG		0x001E0000	/*!< DMAC channel number#5~8  */
//#define CMD_MSK_MEMERR_MTC1		0x000000A0	/*!< CPRI#1 MT用内部メモリ障害割込みマスク値 */
//#define CMD_MSK_MEMERR_MTC2		0x00000050	/*!< CPRI#2 MT用内部メモリ障害割込みマスク値 */
//#endif
//
///****************************************************************************/
///*																			*/
///*				CPRI信号用パラメータデファイン定義							*/
///*																			*/
///****************************************************************************/
///*****************************/
///* RE PORT制御要求パラメータ */
///*****************************/
///* 情報ビット */
//#define	CMD_MIN_INFOBIT			1			/* 最小情報ビット				*/
//#define	CMD_MAX_INFOBIT			8			/* 最大情報ビット				*/
//
///* 制御 */
//#define	CMD_RLYOFF				1			/* リレーOFF					*/
//#define	CMD_RLYON				2			/* リレーON						*/
//
///******************************/
///* REバージョン報告パラメータ */
///******************************/
///* メジャーバージョン/マイナーバージョン */
//#define	CMD_VERNO_MAX			99			/* 最大バージョン番号			*/
//
///* 情報数 */
//#define CMD_VERINFNUM_MAX		16			/* バージョン情報最大数			*/
//
///**********************************/
///* ファイル情報報告通知パラメータ */
///**********************************/
///********************************************/
///* ファイルデータ送信完了報告通知パラメータ */
///********************************************/
///* 対象指定 */
//#define	CMD_FRAME				1			/* 架							*/
//#define	CMD_CARD				3			/* カード						*/
//#define	CMD_FAN					4			/* FAN							*/
//
///* ID情報識別 */
//#define	CMD_HARD				1			/* ハードウェア					*/
//
///* 対象データ */
//#define	CMD_SYSPAR				1			/* システムパラメータ			*/
//#define	CMD_FRM					2			/* ファームウェア				*/
//#define	CMD_FRM_NONEW			3			/* ファームウェア更新なし		*/
//#define	CMD_LOG					4			/* 障害ログ						*/
//
/*********************************/
/*	REスロット情報報告パラメータ */
/*********************************/
/* REカード機能部情報 */
#define	CMD_CRDFNBIT_3GRF		0x1000		/* 3GRF-INF		(D12)			*/
#define	CMD_CRDFNBIT_TRXIF		0x0800		/* TRX-INF		(D11)			*/
#define	CMD_CRDFNBIT_TRX		0x0400		/* TRX			(D10)			*/
#define	CMD_CRDFNBIT_TPA		0x0200		/* T-PA			(D09)			*/
#define	CMD_CRDFNBIT_LNA		0x0100		/* LNA			(D08)			*/
#define	CMD_CRDFNBIT_DUP		0x0080		/* DUP			(D07)			*/
#define	CMD_CRDFNBIT_CLK		0x0040		/* CLK-INF		(D06)			*/
#define	CMD_CRDFNBIT_MT			0x0020		/* MT-INF		(D05)			*/
#define	CMD_CRDFNBIT_PROT		0x0010		/* PORT-INF		(D04)			*/
#define	CMD_CRDFNBIT_TILT		0x0008		/* TILT-INF		(D03)			*/
#define	CMD_CRDFNBIT_EXOARA		0x0004		/* EX-OARA-INF	(D02)			*/
#define	CMD_CRDFNBIT_OARA		0x0002		/* OA-RA-INF	(D01)			*/
#if 0
#define	CMD_CRDFNBIT_VBIT	(\
			CMD_CRDFNBIT_3GRF	|\
			CMD_CRDFNBIT_TRXIF	|\
			CMD_CRDFNBIT_TRX	|\
			CMD_CRDFNBIT_TPA	|\
			CMD_CRDFNBIT_LNA	|\
			CMD_CRDFNBIT_DUP	|\
			CMD_CRDFNBIT_CLK	|\
			CMD_CRDFNBIT_MT		|\
			CMD_CRDFNBIT_PROT	|\
			CMD_CRDFNBIT_TILT	|\
			CMD_CRDFNBIT_EXOARA	|\
			CMD_CRDFNBIT_OARA	)			/* 有効ビット					*/
#endif
#define RED_RESCT_MSK	(\
			CMD_CRDFNBIT_3GRF	|\
			CMD_CRDFNBIT_TRXIF	|\
			CMD_CRDFNBIT_TRX	|\
			CMD_CRDFNBIT_TPA	|\
			CMD_CRDFNBIT_LNA	|\
			CMD_CRDFNBIT_DUP	)			/**<  セクタ部						*/
            
#define RED_RECMN_MSK	(\
			CMD_CRDFNBIT_CLK	|\
			CMD_CRDFNBIT_MT		|\
			CMD_CRDFNBIT_PROT	|\
			CMD_CRDFNBIT_TILT	|\
			CMD_CRDFNBIT_EXOARA	|\
			CMD_CRDFNBIT_OARA	)			/**<  共通部					*/


///* パラメータチェック */
//#define	CMD_SLOTINF_MAX	16					/* スロット情報最大数			*/
//#define	CMD_MINSLOTNO	1					/* 最小SLOT番号					*/
//#define	CMD_MAXSLOTNO	56					/* 最大SLOT番号					*/
//
//
///******************************/
///* REカード状態報告パラメータ */
///******************************/
///* カード基本監視項目 */
//#define	CMD_CRDSVBIT_USE		0x0400		/**<  USE(D10)						 */
//#define	CMD_CRDSVBIT_ALM		0x0200		/**<  ALM(D9)						 */
//#define	CMD_CRDSVBIT_ERR		0x0100		/**<  ERR(D8)						 */
//#define	CMD_CRDSVBIT_EQP		0x0080		/**<  実装(D7)						 */
//#define	CMD_CRDSVBIT_URDY		0x0040		/**<  Not Ready(D6)					 */
//#define	CMD_CRDSVBIT_ACT		0x0020		/**<  ACT(D5)						 */
//#define	CMD_CRDSVBIT_PSTBY		0x0010		/**<  PSTBY(D4)						 */
//#define	CMD_CRDSVBIT_STBY		0x0008		/**<  STBY(D3)						 */
//#define	CMD_CRDSVBIT_PBLK		0x0004		/**<  PBLK(D2)						 */
//#define	CMD_CRDSVBIT_BLK		0x0002		/**<  BLK(D1)						 */
//#define	CMD_CRDSVBIT_CHECK		0x0001		/**<  CHECK(D0)					 	 */
//#define CMD_CRDSVBIT_NOEQP		0x0000		/**<  未実装						 */
#if 0
#define	CMD_CRDSVBIT_VBIT	(\
			CMD_CRDSVBIT_USE	|\
			CMD_CRDSVBIT_ALM	|\
			CMD_CRDSVBIT_ERR	|\
			CMD_CRDSVBIT_EQP	|\
			CMD_CRDSVBIT_URDY	|\
			CMD_CRDSVBIT_ACT	|\
			CMD_CRDSVBIT_PSTBY	|\
			CMD_CRDSVBIT_STBY	|\
			CMD_CRDSVBIT_PBLK	|\
			CMD_CRDSVBIT_BLK	|\
			CMD_CRDSVBIT_CHECK	)			/* 有効ビット					*/

/* カード拡張監視項目 */
#define	CMD_CRDEVBIT_WARMUP		0x0002		/* WarmUp		(D01)			*/

#define	CMD_CRDEVBIT_VBIT	(\
			CMD_CRDEVBIT_WARMUP		)		/* 有効ビット					*/

/* カード制御項目 */
#define	CMD_CRDCTBIT_L_RST		0x4000		/**<  ローカルリセット	(D14)		 */
#define	CMD_CRDCTBIT_L_BLK		0x2000		/**<  ローカル閉塞		(D13)		 */
#define	CMD_CRDCTBIT_L_BLKOFF	0x1000		/**<  ローカル閉塞解除	(D12)		 */
#define	CMD_CRDCTBIT_A_RST		0x0100		/**<  自律リセット		(D08)		 */
#define	CMD_CRDCTBIT_R_RST		0x0040		/**<  リモートリセット	(D06)		 */
#define	CMD_CRDCTBIT_R_BLK		0x0020		/**<  リモート閉塞		(D05)		 */
#define	CMD_CRDCTBIT_R_BLKOFF	0x0010		/**<  リモート閉塞解除	(D04)		 */
#define	CMD_CRDCTBIT_R_NECHG	0x0008		/**<  リモートNE切替	(D03)		 */
#define	CMD_CRDCTBIT_R_CMPBLK	0x0004		/**<  リモート強制閉塞	(D02)		 */
#define	CMD_CRDCTBIT_R_ALM		0x0002		/**<  リモートALM		(D01)		 */
#define	CMD_CRDCTBIT_R_CHK		0x0001		/**<  リモート診断		(D00)		 */

#define	CMD_CRDCTBIT_VBIT	(\
			CMD_CRDCTBIT_L_RST		|\
			CMD_CRDCTBIT_L_BLK		|\
			CMD_CRDCTBIT_L_BLKOFF	|\
			CMD_CRDCTBIT_A_RST		|\
			CMD_CRDCTBIT_R_RST		|\
			CMD_CRDCTBIT_R_BLK		|\
			CMD_CRDCTBIT_R_BLKOFF	|\
			CMD_CRDCTBIT_R_NECHG	|\
			CMD_CRDCTBIT_R_CMPBLK	|\
			CMD_CRDCTBIT_R_ALM		|\
			CMD_CRDCTBIT_R_CHK		)		/* 有効ビット					*/

/* 機能部固有制御項目 */
#define	CMD_FNCCTBIT_MKSPC		0x0800		/* メーカ固有	(D11)			*/
#define	CMD_FNCCTBIT_VBIT	(\
			CMD_FNCCTBIT_MKSPC		)		/* 有効ビット					*/

/* キャリア情報 */
#define	CMD_CARIFBIT_CAR04		0x0800		/* キャリア4	(D11)			*/
#define	CMD_CARIFBIT_CAR03		0x0400		/* キャリア3	(D10)			*/
#define	CMD_CARIFBIT_CAR02		0x0200		/* キャリア2	(D09)			*/
#define	CMD_CARIFBIT_CAR01		0x0100		/* キャリア1	(D08)			*/

#define	CMD_CARIFBIT_VBIT	(\
			CMD_CARIFBIT_CAR04	|\
			CMD_CARIFBIT_CAR03	|\
			CMD_CARIFBIT_CAR02	|\
			CMD_CARIFBIT_CAR01	)			/* 有効ビット					*/
#endif // #if0
///* パラメータチェック */
//#define	CMD_CNTCODEMAX			31			/* 制御コード範囲値				*/
//#define	CMD_CODEMAX				65535		/* 制御コード範囲値				*/
//#define	CMD_YEARMAX				0x9999		/* 年最大値						*/
//#define	CMD_MONDAYMAX			0x9999		/* 月日最大値					*/
//#define	CMD_HOURMAX				0x99		/* 時最大値						*/
//#define	CMD_MINSECMAX			0x9999		/* 分秒最大値					*/
//#define	CMD_CRDALM_INFNUMMAX	255			/* 最大ALM情報数				*/
//#define	CMD_CRDERR_INFNUMMAX	255			/* 最大ERR情報数				*/
#define	CMD_CRDALM_INFMIN		1			/* ALM情報最小値				*/
#define	CMD_CRDALM_INFMAX		255			/* ALM情報最大値				*/
#define	CMD_CRDALM_INFMAX_HIG	5107		/* ALM情報最大値				*/
#define	CMD_CRDERR_INFMIN		1			/* ERR情報最小値				*/
#define	CMD_CRDERR_INFMAX		255			/* ERR情報最大値				*/
#define	CMD_CRDERR_INFMAX_HIG	5107		/* ERR情報最大値				*/
//
///************************/
///* RE状態報告パラメータ */
///************************/
//#define	CMD_FANALM_MAXNUM		4			/* FANALMビット最大数			*/
//#define	CMD_BARIALM_MAXNUM		4			/* バリスタALMビット最大数		*/
//#define	CMD_BRKALM_MAXNUM		2			/* ブレーカALMビット最大数		*/
//
//#define	CMD_FANALM_INF			0x1000		/* FANALM情報報告値				*/
//#define	CMD_BARIALM_INF			0x2000		/* バリスタALM情報報告値		*/
//#define	CMD_BRKALM_INF			0x4000		/* ブレーカALM情報報告値		*/
//
///* パラメータチェック */
//#define	CMD_FANMIN				1		/* FAN情報最小値					*/
//#define	CMD_MK_REMIN			1		/* メーカ固有RE情報最小値			*/
//#define	CMD_NODATA				0		/* テーブル初期値					*/
//
///******************************/
///* REカード制御要求パラメータ */
///******************************/
///* 制御 */
//#define	CMD_CARDRST				1			/**<  リセット						 */
//#define	CMD_CARDBLK				2			/**<  閉塞							 */
//#define	CMD_NOBLK				3			/**<  閉塞解除						 */
//#define	CMD_NECHG				4			/**<  N/E切替						 */
//#define	CMD_ENFBLK				5			/**<  強制閉塞						 */
//#define	CMD_CARDALM				6			/**<  ALM							 */
//
///*****************************************/
///* 無変調キャリア出力ONOFF指定パラメータ */
///*****************************************/
///* 系指定 */
//#define	CMD_BRCNO0				0			/* 0系							*/
//#define	CMD_BRCNO1				1			/* 1系							*/
//
///* 送信電力 */
//#define	CMD_MIN_SNDPWR			350			/* 送信電力最小値				*/
//#define	CMD_MAX_SNDPWR			630			/* 送信電力最大値				*/
//
///****************************/
///* REリセット要求パラメータ	*/
///****************************/	
///* 対象指定	*/
//#define	CMD_RE					0			/* RE							*/
//#define	CMD_LNKALL				1			/* CPRIリンク配下				*/
//
///* RE共通部指定	*/
//#define	CMD_COM_CTIN			0			/* 共通部含む					*/
//#define	CMD_COM_DONCARE			0			/* Don't Care					*/
//#define	CMD_COM_NOCTIN			1			/* 共通部含まない				*/
//
///************************************/
///* RE共通部リンク確立要求パラメータ */
///************************************/
///* 設定 */
//#define	CMD_ACTIVE			0				/* Active						*/
//#define	CMD_NEGATIVE		1				/* Negative						*/
//
/************************************/
/* REキャリア状態報告応答パラメータ */
/************************************/
/* キャリア構成状態	*/
#define	CMD_NMLSND				0			/* 正常送信						*/
#define	CMD_ABNMLSND_0			1			/* 0系送信異常					*/
#define	CMD_ABNMLSND_1			2			/* 1系送信異常					*/

/* Resource Watch */
#define	CMD_RESSTBIT_USE		0x8000		/* 使用中						*/
#define	CMD_RESSTBIT_ALM		0x1000		/* ALM							*/
#define	CMD_RESSTBIT_RSVUSE		0x0400		/* 予備カード使用中				*/

#define CMD_RESSTBIT_VBIT	(\
			CMD_RESSTBIT_USE	|\
			CMD_RESSTBIT_ALM	|\
			CMD_RESSTBIT_RSVUSE	)

///* パラメータチェック */
//#define	CMD_CARSTSMAX			1			/* キャリア状態数最大値			*/
//#define	CMD_CARALM_INFNUMMAX	255			/* 最大キャリアALM情報数		*/
//#define	CMD_CARERR_INFNUMMAX	255			/* 最大キャリアERR情報数		*/
//#define	CMD_CARALM_INFMIN		221			/* キャリアALM最小値			*/
//#define	CMD_CARALM_INFMAX		255			/* キャリアALM最大値			*/
//#define	CMD_CARERR_INFMIN		221			/* キャリアERR最小値			*/
//#define	CMD_CARERR_INFMAX		255			/* キャリアERR最大値			*/
//#define	CMD_CARLMAX				2			/* キャリア最大構成数			*/
//#define	CMD_CARCRDMAX			6			/* キャリアカード最大値			*/
//
//
///************************************/
///* RE装置構成情報報告応答パラメータ */
///************************************/
///* CPRIリンク装置構成情報 			*/
///************************************/
///* BTS形番 */
//#define	CMD_BTS_NO				3001		/* BTS型番						*/
//
///* RE種別 */
//#define	CMD_REKIND_AMP			1			/* AMP							*/
//#define	CMD_REKIND_OFTRX		2			/* OF-TRX						*/
//
///* 3GBTS との共用方式 */
//#define	CMD_MODEL3G_S3G			0			/* S3G 専用						*/
//#define	CMD_MODEL3G_CPRI		1			/* CPRI 共用					*/
//#define	CMD_MODEL3G_RF			2			/* RF 共用  					*/
//#define CMD_MODEL3G_MAX			2			/* 3GBTS との共用方式最大値		*/
//
///* 電源種別 */
//#define	CMD_POWERKIND_DC		0			/* DC							*/
//#define	CMD_POWERKIND_AC1		1			/* AC100V						*/
//#define	CMD_POWERKIND_AC2		2			/* AC200V						*/
//#define	CMD_POWERKIND_AC1_2		3			/* AC100V/AC200V				*/
//
///* パワークラス単位 */
//#define	CMD_POWERUNIT_W			0			/* W							*/
//#define	CMD_POWERUNIT_MW		1			/* mW							*/
//#define	CMD_POWERUNIT_MICROW	2			/* μW							*/
//#define	CMD_POWERUNITMAX		2			/* パワークラス単位最大値		*/
//
///* 設定可能な最大周波数帯域幅 */
//#define CMD_SYSBANDMIN			1			/* 最大周波数帯域幅最小値		*/
//#define	CMD_SYSBAND_5			1			/* 5MHz							*/
//#define	CMD_SYSBAND_10			2			/* 10MHz						*/
//#define	CMD_SYSBAND_15			3			/* 15MHz						*/
//#define	CMD_SYSBAND_20			4			/* 20MHz						*/
//#define	CMD_SYSBANDMAX			4			/* 最大周波数帯域幅最大値		*/
//
///* 対応周波数帯 */
//#define	CMD_FRQBAND_2			0			/* 2GHz帯						*/
//#define	CMD_FRQBAND_8			1			/* 800MHz帯						*/
//#define	CMD_FRQBAND_17			2			/* 1.7GHz帯						*/
//#define	CMD_FRQBAND_2_8			3			/* 2GHz帯/800mHz帯				*/
//#define	CMD_FRQBAND_2_17		4			/* 2GHz帯/1.7GHz帯				*/
//#define	CMD_FRQBAND_8_17		5			/* 800MHz帯/1.7GHz帯			*/
//#define	CMD_FRQBAND_2_8_17		6			/* 2GHz帯/800MHz帯/1.7GHz帯		*/
//#define	CMD_FRQBAND_15			7			/* 1.5GHz						*/
//
///* 800MHz帯フィルタタイプ */
//#define	CMD_800BAND_FLT_A		0			/* Type-A						*/
//#define	CMD_800BAND_FLT_B		1			/* Type-B						*/
///*#define	CMD_800BAND_FLT_AB		2	*/	/* Type-B		TBD				*/
//
///* RE側TRX-INFタイプ情報 */
//#define	CMD_13DB				0			/* 13dB							*/
//#define	CMD_21DB				1			/* 21dB							*/
//
///* パラメータチェック */
//#define	CMD_MAKERMIN			0x41	/* メーカ名最小						*/
//#define	CMD_MAKERMAX			0x5a	/* メーカ名最大						*/
//#define	CMD_EQPMIN				1		/* 装置号番最小						*/
//#define	CMD_EQPMAX				 		/* 装置号番最大						*/
//#define	CMD_PUNITMIN			1		/* T-PA 機能部の最大出力電力		*/
//#define	CMD_2GCARLMAX			1		/* 2GHz帯キャリア数最大値			*/
//#define	CMD_EQUINF_MAX			64		/* 装置構成情報数					*/
//
///********************************/
///* RE障害ログ取得要求パラメータ */
///********************************/
///* RE共通部/REセクタ部指定 */
//#define	CMD_RE_COM				0			/* RE共通部						*/
//#define	CMD_RE_SEC				1			/* REセクタ部					*/
//
//#define	CMD_FILENAME_MAX		256			/* RE障害ログファイル名最大長	*/
//
///********************************/
///* ファイル転送通知パラメータ */
///********************************/
//#define CMD_FILEDAT_MAX			10240		/* ファイル転送サイズ最大値		*/
//
///************************************/
///* 外部装置データ送信要求パラメータ */
///************************************/
//#define CMD_DAT_EXTDATSZ		2048		/* 送信データサイズ最大値		*/
//
///************************************/
///* 外部装置データ受信要求パラメータ */
///************************************/
///* 外部装置番号 */
//#define	CMD_OARA_INF_1			1			/* OA-RA-INF1					*/
//#define	CMD_OARA_INF_2			2			/* OA-RA-INF2					*/
//#define	CMD_MTT_INF				3			/* MTT-INF						*/
//#define	CMD_TILT_INF			4			/* TILT-INF						*/
//
///********************************/
///* MTアドレス設定要求パラメータ */
///********************************/
///* 対象MT */
//#define	CMD_FLD_MT				0			/* FLD-MT						*/
//#define	CMD_SV_MT				1			/* SV-MT						*/
//
///* IPアドレス */
//#define	CMD_IPADD_1				192			/* IPアドレス#1					*/
//#define	CMD_IPADD_2				168			/* IPアドレス#2					*/
//#define	CMD_IPADD_3				1			/* IPアドレス#3					*/
//#define	CMD_IPADD_4				129			/* IPアドレス#4					*/
//
///************************************/
///* FLD-MT/SV-MTデータ転送パラメータ */
///************************************/
//#define CMD_FLDCMD_MXLEN	66560	/* FLD-MT FLD-MTデータコマンド転送最大サイズ	*/
//#define CMD_FLDCMD_BLKLEN	10240	/* FLD-MTデータコマンド転送最大ブロックサイズ	*/
//#define CMD_FLDFIL_MXLEN		4096	/* FLD-MT data file max length 	*/
//#define CMD_SMBINDAT_MXLEN_S3G		10240	/* SV-MT data max length (binary data)	 for S3G	*/
//#define CMD_FLDCMD_MAXBLK	7	/* FLD-MTデータコマンド転送最大ブロック数	*/
//
//
///********************************/
///* カード情報報告応答パラメータ */
///********************************/
///* 上りリソース数 */
//#define	CMD_MDIF_UL_RSCNUM		0			/* MDIF上りリソース数(固定)		*/
//#define	CMD_EXT_UL_RSCNUM		0			/* EXT上りリソース数(固定)		*/
///* 下りリソース数 */
//#define	CMD_MDIF_DL_RSCNUM		0			/* MDIF下りリソース数(固定)		*/
//#define	CMD_EXT_DL_RSCNUM		0			/* EXT下りリソース数(固定)		*/
///* 基本監視項目 */
//#define	CMD_MDIF_BASEITEM		0x03E6		/* MDIF基本監視項目(固定)		*/
//#define	CMD_EXT_BASEITEM		0x03E4		/* EXT基本監視項目(固定)		*/
//#define	CMD_TRA_BASEITEM		0x07E6		/* TRA基本監視項目(固定)		*/
///* 拡張監視項目 */
//#define	CMD_MDIF_EXTITEM		0			/* MDIF拡張監視項目(固定)		*/
//#define	CMD_EXT_EXTITEM			0			/* EXT拡張監視項目(固定)		*/
//#define	CMD_TRA_EXTITEM			0x8002		/* TRA拡張監視項目(固定)		*/
///* 制御項目 */
//#define	CMD_MDIF_CNTITEM		0xEF27		/* MDIF制御項目(固定)			*/
//#define	CMD_EXT_CNTITEM			0xEF07		/* EXT制御項目(固定)			*/
//#define	CMD_TRA_CNTITEM			0xEF27		/* TRA制御項目(固定)			*/
///* 機能部固有情報 */
//#define	CMD_FUNCITEM			0			/* 機能部固有情報				*/
//
//
///***********************************/
///* TOFFSET値/上り/下り装置内遅延値 */
///***********************************/
//
///* TOFFSET値は全装置種別共通値 */
//#define	CMD_TOFFSET_NUM			906			/* TOFFSET値					*/
//
///* 2G-AMP,17G-AMP,800M-AMP 屋内AMPは全て共通 */
//#define	CMD_AMP_UL_NUM			3120		/* 上り装置内遅延量				*/
//#define	CMD_AMP_DL_NUM			4553		/* 下り装置内遅延量				*/
//
///* 2G-OF */
//#define	CMD_2G_OF_1_UL_NUM		3139		/* 上り装置内遅延量				*/
//#define	CMD_2G_OF_1_DL_NUM		4564		/* 下り装置内遅延量				*/
//
///* 17G-OF */
//#define	CMD_17G_OF_UL_NUM		3139		/* 上り装置内遅延量				*/
//#define	CMD_17G_OF_DL_NUM		4583		/* 下り装置内遅延量				*/
//
///* 800M-OF(1キャリア) */
//#define	CMD_800M_OF_UL_NUM		3170		/* 上り装置内遅延量				*/
//#define	CMD_800M_OF_DL_NUM		4580		/* 下り装置内遅延量				*/
//
///* 2G-OF(低出力) */
//#define	CMD_2G_OF_4_UL_NUM		3120		/* 上り装置内遅延量				*/
//#define	CMD_2G_OF_4_DL_NUM		4544		/* 下り装置内遅延量				*/
//
///*****************************/
///* 応答結果全般パラメータ	 */
///*****************************/
//#define	CMD_NML					0			/*	正常						*/
//#define CMD_ENDCORD_CTLNG		1			/*	NG							*/
//#define	CMD_ENDCORD_CTLTO		2			/*	タイムアウト				*/
//#define	CMD_ENDCORD_LNKDSC		3			/*	リンク断					*/
//#define	CMD_ENDCORD_TIMOUT		41			/*	RE応答タイムアウト			*/
//#define	CMD_ENDCORD_REPARNG		42			/*	RE応答パラメータ範囲外		*/
//#define	CMD_ENDCORD_ERR			49			/*	処理NG(REC)					*/
//#define	CMD_ENDCORD_FJT_CPRISTATEFLT 110	/*	CPRI状態異常(メーカ固有)	*/
//
//#define	CMD_HARD_ABNML_RE		50			/*	ハードウェア異常(RE)		*/
//#define	CMD_NG_RE				51			/*	処理NG(RE)					*/
//#define	CMD_KIND_NG_RE			52			/*	対象指定NG(RE)				*/
//#define	CMD_SET_NEGATIVE		53			/*	Negative設定				*/
//#define	CMD_BUFF_FUL			54			/*	バッファフル				*/
//
//#define	CMD_RCVDATSEQNG			60			/*	送信データ順序番号異常		*/
//#define	CMD_BUFF_FUL_REC		60			/*	バッファフル(REC)			*/
//#define	CMD_FRM_NOFILE			60			/*	ファームウェアファイルなし	*/
//#define	CMD_CHKSUM_NG			60			/*	チェックサムNG				*/
//#define	CMD_FUNCSET_ABNML_RE	60			/*	機能部設定異常(RE)			*/
//#define	CMD_LOG_NG				60			/*	障害ログ未対応				*/
//#define	CMD_TO_RE				61			/*	タイムアウト				*/
//#define	CMD_INF_NG 				62			/*	情報NG						*/
//
//#define CMD_SDM_FRM_NOFILE		80			/*	SDMにファームウェアなし		*/
//
//#define	CMD_FMRXDISABLE			100			/*	FLD-MT信号受信不可状態		*/
//#define	CMD_SMRXDISABLE			100			/*	eNB-MT信号受信不可状態		*/
//#define CMD_SVCNT_TO			100			/*	SVCNT T.O.					*/
//#define CMD_FLDCMDRCV_NG		100			/*	FLD-MTコマンド受信異常		*/
//#define CMD_FLDFILRCV_NG		100			/*	FLD-MTファイル受信異常		*/
//#define CMD_SVDATRCV_NG			100			/*	eNB-MT受信異常				*/
//#define	CMD_FLDFILPROT_NG		101			/*	非許容プロトコル			*/
//#define CMD_FLD_TO				102			/*	FLD異常、MDE-Bリセット中	*/
//#define CMD_SV_TO				102			/*	eNB異常						*/
//#define CMD_MULTI_FLDMT			103			/*	他のFLD-MTと接続中			*/
//#define CMD_MULTI_SVMT			103			/*	他のeNB-MTと接続中			*/
//#define	CMD_MAKER_RENG_MAX		220			/* メーカ固有(RE)最大値(220)	*/
//
//#define CMD_APICODE_SYSNG		100			/* System call NG				*/
//
//#define CMD_CPRCODE_SYSNG		100			/* System call NG				*/
//#define CMD_CPRCODE_PARANG		101			/* Parameter out of range		*/
//#define CMD_CPRCODE_SVFTO		102			/* SV-F TO						*/
//#define CMD_CPRCODE_NONPER_PROT	120			/* Nonpermissive protocol		*/
//#define CMD_CPRCODE_MULTI_FLDMT	120			/* already connecting to another FLD-MT	*/
//#define CMD_CPRCODE_MULTI_SVMT	120			/* already connecting to another SV-MT	*/
//
//#define CMD_REKIND_UNMATCH		300			/* RE種別　不一致				*/
//
///****************************************************************************/
///* CPRIリンクインデックス定義												*/
///****************************************************************************/
//#define	CMD_CPRIIDX_00			0			/* CPRIリンクインデックス# 0	*/
//#define	CMD_CPRIIDX_01			1			/* CPRIリンクインデックス# 1	*/
//#define	CMD_CPRIIDX_02			2			/* CPRIリンクインデックス# 2	*/
//#define	CMD_CPRIIDX_03			3			/* CPRIリンクインデックス# 3	*/
//#define	CMD_CPRIIDX_04			4			/* CPRIリンクインデックス# 4	*/
//#define	CMD_CPRIIDX_05			5			/* CPRIリンクインデックス# 5	*/
//#define	CMD_CPRIIDX_MAX			6			/* CPRIリンクインデックス最大数	*/
//
///****************************************************************************/
///* セクタ番号定義															*/
///****************************************************************************/
//#define	CMD_SECTNO_MIN			1			/* セクタ番号最小数				*/
//#define	CMD_SECTNO_01			1			/* セクタ番号# 1				*/
//#define	CMD_SECTNO_02			2			/* セクタ番号# 2				*/
//#define	CMD_SECTNO_03			3			/* セクタ番号# 3				*/
//#define	CMD_SECTNO_04			4			/* セクタ番号# 4				*/
//#define	CMD_SECTNO_05			5			/* セクタ番号# 5				*/
//#define	CMD_SECTNO_06			6			/* セクタ番号# 6				*/
//#define	CMD_SECTNO_MAX			6			/* セクタ番号最大数				*/
//
/****************************************************************************/
/* キャリア番号定義															*/
/****************************************************************************/
#define	CMD_CARNO_MIN			1			/* キャリア番号最小数			*/
#define CMD_CARNO_01            1           /* Carrier No #1        */  
#define	CMD_CARNO_MAX			2			/* キャリア番号最大数			*/
#define	CMD_CARNO_MAX_3G		4			/* キャリア番号最大数(3G)		*/
///****************************************************************************/
///*																			*/
///*							TRIF共通関数定義								*/
///*																			*/
///****************************************************************************/
///****************************************************************************/
///* Active状態チェック処理(TRIF)												*/
///****************************************************************************/
//#define CMD_STA_ACTIVE  		0		/* Active状態						*/
//#define CMD_STA_NEGATIVE 		1		/* Negative状態						*/
//
///****************************************************************************/
///* return code for active switch request (t_cm_ActSwReq / t_cm_RECLSReq )	*/
///****************************************************************************/
//#define CMD_ACTSW_NOTCONFIG		1	/* not config link					*/
//#define CMD_ACTSW_OFTRX			2	/* request link is OFTRX link	    */
//#define CMD_ACTSW_DOUBLE		3	/* other active switch is running	*/
//#define CMD_ACTSW_NOTACT		4	/* input link is not active link	*/
//
///****************************************************************************/
///* カード状態監視関連定義													*/
///****************************************************************************/
///*	自カード状態テーブル/他カード状態テーブル */
///* カード状態 */
//#define	CMD_CRDSTA_NOINS		0		/*	未実装							*/
//#define	CMD_CRDSTA_INS			1		/*	実装							*/
//#define	CMD_CRDSTA_ACT			2		/*	ACT								*/
//#define	CMD_CRDSTA_STBY			3		/*	STBY							*/
//#define	CMD_CRDSTA_PSTBY		4		/*	PSTBY							*/
//#define	CMD_CRDSTA_BLK			5		/*	BLK								*/
//#define	CMD_CRDSTA_PBLK			6		/*	PBLK							*/
//#define CMD_CRDSTA_TEST			7		/*	TEST							*/
//#define	CMD_CRDSTA_ALM			8		/*	ALM								*/
//
///****************************************************************************/
///*																			*/
///*				タスク間インタフェース詳細内の共通定義						*/
///*																			*/
///****************************************************************************/
/* REへファイル更新種別 */
#define	CMD_FILUP_TYPE_STA		0		/* 起動中REへファイル更新 */
#define	CMD_FILUP_TYPE_USE		1		/* 運用中REへファイル更新 */

///* 結果コード */
//#define	CMD_RLTCODE_OK			0		/* 正常								*/
//#define	CMD_RLTCODE_NG			1		/* 異常								*/
//#define CMD_RLTCODE_OAMNG		1		/* the NG case of OAM */
//#define CMD_RLTCODE_RENG		2		/* the NG case of RE */
//#define	CMD_RLTCODE_NONFIRM		2		/* ファームウェアファイルなし		*/
//#define	CMD_RLTCODE_SDMNONFIL	3		/* SDM内に該当ファイルなし			*/
//
///* REリセット要否 */
//#define	CMD_RSTJDG_NONRESET		0		/* リセット不要						*/
//#define	CMD_RSTJDG_RESET		1		/* リセット要						*/
//
///*	L3信号転送処理送信用オブジェクトフラグ */
//#define	CMD_OBJFLG_CPRI			0		/* CPRI指定							*/
//#define	CMD_OBJFLG_AMP			1		/* AMP指定							*/
//
///* Active切替結果 */
//#define	CMD_CHGRLT_CHGNG		1		/* 切替NG							*/
//#define	CMD_CHGRLT_CHGFBD		2		/* 切替禁止							*/
//#define	CMD_CHGRLT_INVACT		3		/* 無効な新activeリンク指定			*/
//#define	CMD_CHGRLT_NEGACTNG		4		/* active switch is running			*/
//
///* 切替トリガー */
#define	CMD_CHGTRG_NEGCHG		1		/* 45:Negative設定					*/
//#define	CMD_CHGTRG_MTCOM		2		/* MTコマンド						*/
//#define	CMD_CHGTRG_NEGRXLINK	3		/* negative RXリンク検出に応じる	*/
//#define	CMD_CHGTRG_REACTREQ		4		/* 再active依頼通知に応じる			*/
//#define	CMD_CHGTRG_L1DSN		5		/* L1 断							*/
//#define	CMD_CHGTRG_TXSNDNG		6		/* TX送信異常						*/
//#define	CMD_CHGTRG_HDLCIDLE		7		/* HDLC IDLE						*/
//#define	CMD_CHGTRG_L2DSN		8		/* L2断								*/
//#define	CMD_CHGTRG_HELDSN		9		/* ヘルスチェック断					*/
//#define	CMD_CHGTRG_RELEAD		10		/* RE主導							*/
//#define	CMD_CHGTRG_RECMPSTP		11		/* RE強制停止通知					*/
//#define	CMD_CHGTRG_RERST		12		/* REリセット						*/
//#define CMD_CHGTRG_COMLNKFAIL	13		/* RE共通部リンク確立失敗			*/
//#define CMD_CHGTRG_NECHG		14		/* N/E切替再起動					*/
//
///* リンク状態 */
//#define D_TCM_STA_A   		0x0001   /* CPRI stateA */
//#define D_TCM_STA_B   		0x0002   /* CPRI stateB */
//#define D_TCM_STA_C   		0x0004   /* CPRI stateC */
//#define D_TCM_STA_D   		0x0008   /* CPRI stateD */
//#define D_TCM_STA_E   		0x0010   /* CPRI stateE */
//#define D_TCM_STA_EDWN  	0x0020   /* CPRI stateE donw */
//#define D_TCM_STA_HDLCIDLE 	0x0040   /* HDLC IDLE   */
//#define D_TCM_STA_HDLCACT  	0x0080   /* HDLC ACT   */
//#define D_TCM_STA_F         0x0100   /*CPRI stateF */
//#define D_TCM_STA_L3_OK     0x0200   /*CPRI state L3 OK */
//#define D_TCM_STA_INVALID   0xFFFF   /*CPRI state invalid */
//
//
///* 「RE共通部確立」 cause 定義 */
//#define CMD_RECLS_REQ			14		/* RE共通部リンク確立要求			*/
//
///* Negative notification type定義 for task IF Negative通知 */
//#define CMD_NEGNTC_TYEP_ACTDSC		1	/* Activeリンク->Negative通知		*/
//#define CMD_NEGNTC_TYEP_ACTDSCTO	2	/* Active断カード状態安定化時間TO	*/
//
///* request type 定義  for task IF active switch forbid request */
//#define CMD_ACTSW_FORBON		1		/* active switch forbid on			*/
//#define CMD_ACTSW_FORBOFF		2		/* active switch forbid off			*/
//
///* 更新結果 */
//#define CMD_FUPRSL_NON			0			/* 更新なし						*/
//#define CMD_FUPRSL_FW			1			/* FW更新						*/
//#define CMD_FUPRSL_FPGA			2			/* FPGA更新						*/
//#define CMD_FUPRSL_FWFP			3			/* FW+FPGA更新					*/
//#define CMD_FUPRSL_NG			4			/* 更新失敗						*/
//
/* RE kind 定義 for task IF CPRIリンクstate通知  */
#define CMD_RETYPE_AMP1			1			/* AMP 1						*/
#define CMD_RETYPE_OFTRX		3			/*!< OFTRX						*/
//
///* バージョン結果通知 */
//#define	CMD_MKNAME_MAX			16		/* メーカ名の配列最大数				*/
//#define	CMD_IDINF_MAX			1024	/* ID情報の配列最大数				*/
//#define	CMD_NAME_MAX			10		/* 名称の配列最大数					*/
//#define	CMD_VERSION_MAX			256		/* バージョン情報の構造体最大数		*/
//
///* REカード情報報告 */
//#define	CMD_CRDNAME_MAX			8		/* カード名の配列最大数				*/
//
///* REカード診断要求 */
//#define	CMD_DIARSLT_NG			0		/* 診断結果NG						*/
//
///* RE PORT制御要求 */
///* 情報ビット */
//#define	CMD_INFBIT_MIN			1		/* 情報ビット最小値					*/
//#define	CMD_INFBIT_MAX			8		/* 情報ビット最大値					*/
//
///* 制御 */
//#define	CMD_PORT_ROFF			1		/* リレーOFF						*/
//#define	CMD_PORT_RON			2		/* リレーON							*/
//
///* REC/RE種別 */
//#define	CMD_RECRE_REC			0		/*	REC								*/
//#define	CMD_RECRE_AMP			1		/*	AMP								*/
//
///* REカード制御要求 */
///* 制御コード */
//#define	CMD_CNTCODE_MIN			0		/*	制御コード最少値				*/
//#define	CMD_CNTCODE_MAX			31		/*	制御コード最大値				*/
//
///* CPRIリンクリセット要求 */
///* リセット種別 */
//#define	CMD_SOFTRST				1		/*	ソフトリセット					*/
//#define	CMD_HARDRST				2		/*	ハードリセット					*/
//
///* 発行マクロ */
//#define CMD_CPRILNKRST_1		1		/* CPRIリンクリセットマクロ			*/
//#define CMD_CPRILNKRST_2		2		/* CPRIリンクリセットマクロ2		*/
//
///* キャリア制御要求 */
///* 制御 */
//#define CMD_CNT_RST				1		/* 休止								*/
//#define CMD_CNT_RSTCCL			2		/* 休止解除							*/
//
///* TRX設定通知 */
///* 周波数番号 */
//#define	CMD_FRQA_MIN			9612	/* 周波数番号A最小値				*/
//#define	CMD_FRQA_MAX			9888	/* 周波数番号A最大値				*/
//#define	CMD_FRQB_MIN			4162	/* 周波数番号B最小値				*/
//#define	CMD_FRQB_MAX			4188	/* 周波数番号B最大値				*/
//#define	CMD_FRQC_A				812		/* 周波数番号C_A					*/
//#define	CMD_FRQC_B				837		/* 周波数番号C_B					*/
//#define	CMD_FRQD_MIN			8562	/* 周波数番号D最小値				*/
//#define	CMD_FRQD_MAX			8913	/* 周波数番号D最大値				*/
//
///* primary scrambling code */
//#define	CMD_PSC_MIN				0		/* primary scrambling code最小値	*/
//#define	CMD_PSC_MAX				512		/* primary scrambling code最大値	*/
//
///* Tcell */
//#define	CMD_TCELL_MIN			0		/* Tcell最小値						*/
//#define	CMD_TCELL_MAX			9		/* Tcell最大値						*/
//
///* N/E切替完了/データ引継ぎ完了処理種別 */
//#define	CMD_CHGCMPKIND_CMP		0		/* 完了								*/
//#define	CMD_CHGCMPKIND_CNL		1		/* 中止								*/
//#define	CMD_CHGCMPKIND_ERR		2		/* 失敗								*/
//
///****************************************************************************/
///* ダウンロード管理関連定義													*/
///****************************************************************************/
///*	ファイル送信状態														*/
//#define	CMD_DLSNDSTS_INIT		0		/*	初期状態						*/
//#define	CMD_DLSNDSTS_SPPREFWD	1		/*	システムパラメータ転送準備中	*/
//#define	CMD_DLSNDSTS_SPFWD		2		/*	システムパラメータ転送中		*/
//#define	CMD_DLSNDSTS_FIVERGET	3		/*	REバージョン取得中				*/
//#define	CMD_DLSNDSTS_FIFILGET	4		/*	REファーム取得中				*/
//#define	CMD_DLSNDSTS_FIPREFWD	5		/*	REファーム転送準備中			*/
//#define	CMD_DLSNDSTS_FIFWD		6		/*	REファーム転送中				*/
//
//#define	CMD_DLSNDSTS_MAX		7		/*	ファイル送信状態総数			*/
//
//
///*	CPRIリンク用0x100オフセット		*/
//#define	CMD_CPR01_OFST100		0x0000		/* CPRIリンク#01オフセット		*/
//#define	CMD_CPR02_OFST100		0x0100		/* CPRIリンク#02オフセット		*/
//#define	CMD_CPR03_OFST100		0x0200		/* CPRIリンク#03オフセット		*/
//#define	CMD_CPR04_OFST100		0x0300		/* CPRIリンク#04オフセット		*/
//#define	CMD_CPR05_OFST100		0x0400		/* CPRIリンク#05オフセット		*/
//#define	CMD_CPR06_OFST100		0x0500		/* CPRIリンク#06オフセット		*/
//
//
//
///*	CPRIリンク用0x0020オフセット		*/
//#define	CMD_CPR01_OFST20		0x000000		/* CPRIリンク#01オフセット		*/
//#define	CMD_CPR02_OFST20		0x000020		/* CPRIリンク#02オフセット		*/
//#define	CMD_CPR03_OFST20		0x000040		/* CPRIリンク#03オフセット		*/
//#define	CMD_CPR04_OFST20		0x020000		/* CPRIリンク#04オフセット		*/
//#define	CMD_CPR05_OFST20		0x020020		/* CPRIリンク#05オフセット		*/
//#define	CMD_CPR06_OFST20		0x020040		/* CPRIリンク#06オフセット		*/
//
//
///****************************************************************************/
///*CPRIスタートUP制御レジスタ*/
///****************************************************************************/
///*	ベースアドレス	*/
//#ifdef _FORIT1_
//#define	CMD_CPRISTATE_ST_BASE	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[0]
//#else
///*Old Addr=0x300004E0 New Addr=0x00820204*/
//#define	CMD_CPRISTATE_ST_BASE	0x00820204	
//#endif
//
///*	アドレス	*/
//
//#ifdef _FORIT1_
///*	CPRI#01～#06				*/
//#define	CMD_CPRISTATE_ST_CPR01	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[0]
//#define	CMD_CPRISTATE_ST_CPR02	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[1]
//#define	CMD_CPRISTATE_ST_CPR03	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[2]
//#define	CMD_CPRISTATE_ST_CPR04	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[3]
//#define	CMD_CPRISTATE_ST_CPR05	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[4]
//#define	CMD_CPRISTATE_ST_CPR06	(USHORT *)&IT1DMY_CMD_CPRISTATE_ST_CPR[5]
//
//#else
///*	CPRI#01～#06				*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_CPRISTATE_ST_CPR01	0x00201004
//#define	CMD_CPRISTATE_ST_CPR02	0x00201104
//#define	CMD_CPRISTATE_ST_CPR03	0x00201204
//#define	CMD_CPRISTATE_ST_CPR04	0x00201304
//#define	CMD_CPRISTATE_ST_CPR05	0x00201404
//#define	CMD_CPRISTATE_ST_CPR06	0x00201504
//#else
//#define	CMD_CPRISTATE_ST_CPR01	D_CMM_FPGA_ADDR_CPRI_1_STUP_LTE	/*	0x00282000	*/
//#define	CMD_CPRISTATE_ST_CPR02	D_CMM_FPGA_ADDR_CPRI_2_STUP_LTE	/*	0x00282010	*/
//#define	CMD_CPRISTATE_ST_CPR03	0x00000000
//#define	CMD_CPRISTATE_ST_CPR04	0x00000000
//#define	CMD_CPRISTATE_ST_CPR05	0x00000000
//#define	CMD_CPRISTATE_ST_CPR06	0x00000000
//#endif
//
//#endif
//	
//
///****************************************************************************/
///*																			*/
///*				TRIFカード内共通レジスタアドレス/マスク定義					*/
///*																			*/
///****************************************************************************/
///*	CPRIリンク用0x400オフセット		*/
///*FFCS)Linlj 2007/03/19 Modify Start*/
//#define	CMD_CPR01_OFST400		0x000000		/* CPRIリンク#01オフセット		*/
//#define	CMD_CPR02_OFST400		0x000200		/* CPRIリンク#02オフセット		*/
//#define	CMD_CPR03_OFST400		0x000400		/* CPRIリンク#03オフセット		*/
//#define	CMD_CPR04_OFST400		0x020000		/* CPRIリンク#04オフセット		*/
//#define	CMD_CPR05_OFST400		0x020200		/* CPRIリンク#05オフセット		*/
//#define	CMD_CPR06_OFST400		0x020400		/* CPRIリンク#06オフセット		*/
///*FFCS)Linlj 2007/03/19 Modify End*/
//
//
//#define	CMD_CPRINT_OFST400		0x0400	/* CPRIリンク用0x400オフセット間隔	*/
//
///****************************************************************************/
///* CPRI stateF 制御レジスタ													*/
///****************************************************************************/
///*	ベースアドレス	*/
//#ifdef _FORIT1_
//#define	CMD_REGADR_STAFCNT_BASE	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[0]	/*	CPRI stateF 制御	*/
//#else
///*Old Addr=0x3000120E New Addr=0x0082020C*/
//#define	CMD_REGADR_STAFCNT_BASE	0x0082020C	/*	CPRI stateF 制御	*/
//#endif
//
//
///*	アドレス	*/
//#ifdef _FORIT1_
//											/*	CPRI#01～#06				*/
//#define	CMD_REGADR_STAFCNT_CPR01	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[0]
//#define	CMD_REGADR_STAFCNT_CPR02	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[1]
//#define	CMD_REGADR_STAFCNT_CPR03	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[2]
//#define	CMD_REGADR_STAFCNT_CPR04	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[3]
//#define	CMD_REGADR_STAFCNT_CPR05	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[4]
//#define	CMD_REGADR_STAFCNT_CPR06	(USHORT *)&IT1DMY_CMD_REGADR_STAFCNT_CPR[5]
//
//#else
//											/*	CPRI#01～#06				*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_STAFCNT_CPR01	0x00201008
//#define	CMD_REGADR_STAFCNT_CPR02	0x00201108
//#define	CMD_REGADR_STAFCNT_CPR03	0x00201208
//#define	CMD_REGADR_STAFCNT_CPR04	0x00201308
//#define	CMD_REGADR_STAFCNT_CPR05	0x00201408
//#define	CMD_REGADR_STAFCNT_CPR06	0x00201508
//#else
//#define	CMD_REGADR_STAFCNT_CPR01	0x00000000
//#define	CMD_REGADR_STAFCNT_CPR02	0x00000000
//#define	CMD_REGADR_STAFCNT_CPR03	0x00000000
//#define	CMD_REGADR_STAFCNT_CPR04	0x00000000
//#define	CMD_REGADR_STAFCNT_CPR05	0x00000000
//#define	CMD_REGADR_STAFCNT_CPR06	0x00000000
//#endif
//
//#endif
//
///****************************************************************************/
///* CPRI ステート表示レジスタ												*/
///****************************************************************************/
///*	ベースアドレス	*/
//#ifdef _FORIT1_
//#define	CMD_REGADR_CPRISTA_BASE	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[0]	/*	CPRI ステート表示レジスタ 	*/
//#else
///*Old Addr=0x30001206 New Addr=0x00820208*/
//#define	CMD_REGADR_CPRISTA_BASE	0x00820208	/*	CPRI ステート表示レジスタ 	*/
//#endif
//
//
///*	アドレス	*/
//#ifdef _FORIT1_
//											/*	CPRI#01～#06				*/
//#define	CMD_REGADR_CPRISTA_CPR01	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[0]
//#define	CMD_REGADR_CPRISTA_CPR02	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[1]
//#define	CMD_REGADR_CPRISTA_CPR03	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[2]
//#define	CMD_REGADR_CPRISTA_CPR04	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[3]
//#define	CMD_REGADR_CPRISTA_CPR05	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[4]
//#define	CMD_REGADR_CPRISTA_CPR06	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[5]
//
//#else
///* CPRIリンク#1 ~ #6 State表示レジスタ				*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_CPRISTA_CPR01	0x0020011C
//#define	CMD_REGADR_CPRISTA_CPR02	0x0020012C
//#define	CMD_REGADR_CPRISTA_CPR03	0x0020013C
//#define	CMD_REGADR_CPRISTA_CPR04	0x0020014C
//#define	CMD_REGADR_CPRISTA_CPR05	0x0020015C
//#define	CMD_REGADR_CPRISTA_CPR06	0x0020016C
//#else
//#define	CMD_REGADR_CPRISTA_CPR01	D_CMM_FPGA_ADDR_CPRI_1_STATE	/*	0x00282300	*/
//#define	CMD_REGADR_CPRISTA_CPR02	D_CMM_FPGA_ADDR_CPRI_2_STATE	/*	0x00282400	*/
//#define	CMD_REGADR_CPRISTA_CPR03	0x00000000
//#define	CMD_REGADR_CPRISTA_CPR04	0x00000000
//#define	CMD_REGADR_CPRISTA_CPR05	0x00000000
//#define	CMD_REGADR_CPRISTA_CPR06	0x00000000
//#endif
//
//#endif
//
///****************************************************************************/
///* Active / idle channel state indication register							*/
///****************************************************************************/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_HDLC_T4	0x00200020	/* HDLC T4 set register*/
//#else
//#define	CMD_REGADR_HDLC_T4	D_CMM_FPGA_ADDR_HDLC_T4_SET	/* HDLC T4 set register*//*	0x00282204	*/
//#endif
//
///****************************************************************************/
///* CPRI state define, which is get from CPRIステート表示レジスタ */
///* アクティブ／アイドルチャネル状態表示レジスタ						*/
///****************************************************************************/
//#define	CRD_CPRISTA_A				0   /* CPRI stateA */
//#define	CRD_CPRISTA_B				1   /* CPRI stateB */
//#define	CRD_CPRISTA_C				2   /* CPRI stateC */
//#define	CRD_CPRISTA_D				3   /* CPRI stateD */
//#define	CRD_CPRISTA_E_IDLE		4   /* CPRI stateE(IDLE) */
//#define	CRD_CPRISTA_E_ACT		5   /* CPRI stateE(ACT) */
//#define	CRD_CPRISTA_F				6   /* CPRI stateF */
//
///*	設定値	*/
//#define	CMD_REGSET_L2ERR		0x0001		/*	L2ERR						*/
//#define	CMD_REGSET_L3ERR		0x0002		/*	L3ERR						*/
//#define	CMD_REGSET_DELAYNG		0x0004		/*	遅延NG						*/
//#define	CMD_REGSET_L2ERROFF		0x0006		/*	L2ERR OFF					*/
//#define	CMD_REGSET_L3ERROFF		0x0005		/*	L3ERR OFF					*/
//
///*!
// *  @brief  LAPB protocol L2 parameter of  DoCoMo specification
// */	
//#define			LBD_L2PRMTR_N1		0x280E		/*!< 10254*/
//#define			LBD_L2PRMTR_N2		0x5
//#define			LBD_L2PRMTR_K		0x7
//#define			LBD_L2PRMTR_T1		0xF		    /*!< 50 ** 10ms unit	*/	
//#define			LBD_L2PRMTR_T2		0x5			/*!< 5  ** 10ms unit	*/	
//#define			LBD_L2PRMTR_T3		0x64		/*!< 100 **	10ms unit   */	
//#define			LBD_L2PRMTR_T4		0x3E8		/*!< 1000 ** 10ms unit  */	
//
///*FFCS)Linlj 2007/03/19 Add Start*/
///****************************************************************************/
///* CPRI ステート change表示レジスタ												*/
///****************************************************************************/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_CPRSTA_INTER_DSP		0x00200100	/*	CPRIリンクState状変割込み表示レジスタ */
//#define	CMD_REGADR_CPRSTA_INTER_MASK	0x00200104	/*	CPRIリンクState状変割込みマスクONレジスタ */
//#define	CMD_REGADR_CPRSTA_INTER_UNMASK	0x00200108	/*	CPRIリンクState状変割込みマスクOFFレジスタ */
//#else
//#define	CMD_REGADR_CPRSTA_INTER_DSP		D_CMM_FPGA_ADDR_CPRI_STATE_INT_LTE	/*	CPRIリンクState状変割込み表示レジスタ *//*	0x00281100	*/
//#define	CMD_REGADR_CPRSTA_INTER_MASK	D_CMM_FPGA_ADDR_CPRI_STATE_INT_ON_LTE	/*	CPRIリンクState状変割込みマスクONレジスタ *//*	0x00281104	*/
//#define	CMD_REGADR_CPRSTA_INTER_UNMASK	D_CMM_FPGA_ADDR_CPRI_STATE_INT_OFF_LTE	/*	CPRIリンクState状変割込みマスクOFFレジスタ *//*	0x00281108	*/
//#endif	
//
//#define	CMD_REGMSK_CPRSTA				0x00000003	/*	CPRI#1/#2有効			*/
//
///*	アドレス	*/
///*FFCS)Linlj 2007/03/19 Add Start*/
//#ifdef _FORIT1_
///*	CPRIリンク#1 ~ #6 State状変マスクONレジスタ				*/
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR01	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[0]
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR02	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[1]
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR03	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[2]
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR04	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[3]
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR05	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[4]
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR06	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[5]
//#else
///*	CPRIリンク#1 ~ #6 State状変マスクONレジスタ				*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR01	0x00200114
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR02	0x00200124
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR03	0x00200134
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR04	0x00200144
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR05	0x00200154
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR06	0x00200164
//#else
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR01	D_CMM_FPGA_ADDR_CPRI_1_STATE_INT_ON_LTE	/*	0x00281114	*/
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR02	D_CMM_FPGA_ADDR_CPRI_2_STATE_INT_ON_LTE	/*	0x00281124	*/
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR03	0x00000000
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR04	0x00000000
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR05	0x00000000
//#define	CMD_REGADR_CPRSTACHG_MASK_CPR06	0x00000000
//#endif
//
//
//#endif
//
//#ifdef _FORIT1_
///*	CPRIリンク#1 ~ #6 State状変マスクOFFレジスタ				*/
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR01
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR02
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR03
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR04
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR05
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR06
//#else
//
///*	CPRIリンク#1 ~ #6 State状変マスクOFFレジスタ				*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR01	0x00200118
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR02	0x00200128
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR03	0x00200138
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR04	0x00200148
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR05	0x00200158
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR06	0x00200168
//#else
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR01	D_CMM_FPGA_ADDR_CPRI_1_STATE_INT_OFF_LTE	/*	0x00281118	*/
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR02	D_CMM_FPGA_ADDR_CPRI_2_STATE_INT_OFF_LTE	/*	0x00281128	*/
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR03	0x00000000
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR04	0x00000000
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR05	0x00000000
//#define	CMD_REGADR_CPRSTACHG_UNMASK_CPR06	0x00000000
//#endif
//
//#endif
//
//
//#ifdef _FORIT1_
///*CPRIリンク State状変表示レジスタ 	CPRI#1～#6				*/
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR01	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[0]
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR02	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[1]
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR03	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[2]
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR04	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[3]
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR05	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[4]
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR06	(USHORT *)&IT1DMY_CMD_REGADR_CPRISTA_CPR[5]
//#else
//
///*CPRIリンク State状変表示レジスタ 	CPRI#1～#6				*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR01	0x00200110
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR02	0x00200120
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR03	0x00200130
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR04	0x00200140
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR05	0x00200150
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR06	0x00200160
//#else
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR01	D_CMM_FPGA_ADDR_CPRI_1_STATE_INT_LTE	/*	0x00281110	*/
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR02	D_CMM_FPGA_ADDR_CPRI_2_STATE_INT_LTE	/*	0x00281120	*/
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR03	0x00000000
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR04	0x00000000
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR05	0x00000000
//#define	CMD_REGADR_CPRSTACHG_DSP_CPR06	0x00000000
//#endif
//
//
//#endif
//
///*FFCS)Linlj 2007/03/19 Add End*/
//
///* CPRI リンク使用/未使用 レジスタ */
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGADR_CPRIENB_INTER_DSP	0x00200000	/* CPRI リンク使用/未使用 割込み要因レジスタ */
//#define CMD_REGADR_CPRIENB_MASK			0x00200004	/* CPRI リンク使用/未使用 割込み要因マスクONレジスタ */
//#define CMD_REGADR_CPRIENB_UNMASK		0x00200008	/* CPRI リンク使用/未使用 割込み要因マスクOFFレジスタ */
//#define CMD_REGADR_CPRIENB_NEW_STA		0x0020000C	/* CPRI リンク使用/未使用 新表示レジスタ */
//#define CMD_REGADR_CPRIENB_OLD_STA		0x00200010	/* CPRI リンク使用/未使用 旧表示レジスタ */
//#else
//#define	CMD_REGADR_CPRIENB_INTER_DSP	0x00000000	/* CPRI リンク使用/未使用 割込み要因レジスタ */
//#define CMD_REGADR_CPRIENB_MASK			0x00000000	/* CPRI リンク使用/未使用 割込み要因マスクONレジスタ */
//#define CMD_REGADR_CPRIENB_UNMASK		0x00000000	/* CPRI リンク使用/未使用 割込み要因マスクOFFレジスタ */
//#define CMD_REGADR_CPRIENB_NEW_STA		D_CMM_FPGA_ADDR_CPRI_USE	/* CPRI リンク使用/未使用 新表示レジスタ *//*	0x00282200	*/
//#define CMD_REGADR_CPRIENB_OLD_STA		0x00000000	/* CPRI リンク使用/未使用 旧表示レジスタ */
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_RCVSTA_CPR01			0x00200040
//#define CMD_REGADR_RCVSTA_CPR02			0x00200044
//#define CMD_REGADR_RCVSTA_CPR03			0x00200048
//#define CMD_REGADR_RCVSTA_CPR04			0x0020004C
//#define CMD_REGADR_RCVSTA_CPR05			0x00200050
//#define CMD_REGADR_RCVSTA_CPR06			0x00200054
//#else
//#define CMD_REGADR_RCVSTA_CPR01			D_CMM_FPGA_ADDR_CPRI_1_HDLC_ERR_LTE	/*	0x0028200C	*/
//#define CMD_REGADR_RCVSTA_CPR02			D_CMM_FPGA_ADDR_CPRI_2_HDLC_ERR_LTE	/*	0x0028201C	*/
//#define CMD_REGADR_RCVSTA_CPR03			0x00000000
//#define CMD_REGADR_RCVSTA_CPR04			0x00000000
//#define CMD_REGADR_RCVSTA_CPR05			0x00000000
//#define CMD_REGADR_RCVSTA_CPR06			0x00000000
//#endif
//
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_RCVDES_CPR01			0x00202700
//#define CMD_REGADR_RCVDES_CPR02			0x00202800
//#define CMD_REGADR_RCVDES_CPR03			0x00202900
//#define CMD_REGADR_RCVDES_CPR04			0x00202A00
//#define CMD_REGADR_RCVDES_CPR05			0x00202B00
//#define CMD_REGADR_RCVDES_CPR06			0x00202C00
//#else
//#define CMD_REGADR_RCVDES_CPR01			D_CMM_FPGA_ADDR_DMAC3_DSCADD	/*	0x00103300	*/
//#define CMD_REGADR_RCVDES_CPR02			D_CMM_FPGA_ADDR_DMAC4_DSCADD	/*	0x00103400	*/
//#define CMD_REGADR_RCVDES_CPR03			D_CMM_FPGA_ADDR_DMAC5_DSCADD	/*	0x00103500	*//*	CPRIトレース送信データCPRI01	*/
//#define CMD_REGADR_RCVDES_CPR04			D_CMM_FPGA_ADDR_DMAC6_DSCADD	/*	0x00103600	*//*	CPRIトレース送信データCPRI02	*/
//#define CMD_REGADR_RCVDES_CPR05			D_CMM_FPGA_ADDR_DMAC7_DSCADD	/*	0x00103700	*//*	CPRIトレース受信データCPRI01	*/
//#define CMD_REGADR_RCVDES_CPR06			D_CMM_FPGA_ADDR_DMAC8_DSCADD	/*	0x00103800	*//*	CPRIトレース受信データCPRI02	*/
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_RCVORD_CPR01			0x00202704
//#define CMD_REGADR_RCVORD_CPR02			0x00202804
//#define CMD_REGADR_RCVORD_CPR03			0x00202904
//#define CMD_REGADR_RCVORD_CPR04			0x00202A04
//#define CMD_REGADR_RCVORD_CPR05			0x00202B04
//#define CMD_REGADR_RCVORD_CPR06			0x00202C04
//#else
//#define CMD_REGADR_RCVORD_CPR01			D_CMM_FPGA_ADDR_DMAC3_ODR		/*	0x00103304	*/
//#define CMD_REGADR_RCVORD_CPR02			D_CMM_FPGA_ADDR_DMAC4_ODR		/*	0x00103404	*/
//#define CMD_REGADR_RCVORD_CPR03			D_CMM_FPGA_ADDR_DMAC5_ODR		/*	0x00103504	*//*	CPRIトレース送信データCPRI01	*/
//#define CMD_REGADR_RCVORD_CPR04			D_CMM_FPGA_ADDR_DMAC6_ODR		/*	0x00103604	*//*	CPRIトレース送信データCPRI02	*/
//#define CMD_REGADR_RCVORD_CPR05			D_CMM_FPGA_ADDR_DMAC6_ODR		/*	0x00103704	*//*	CPRIトレース受信データCPRI01	*/
//#define CMD_REGADR_RCVORD_CPR06			D_CMM_FPGA_ADDR_DMAC6_ODR		/*	0x00103804	*//*	CPRIトレース受信データCPRI02	*/
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_RCVIOSTA_CPR01		0x00202708
//#define CMD_REGADR_RCVIOSTA_CPR02		0x00202808
//#define CMD_REGADR_RCVIOSTA_CPR03		0x00202908
//#define CMD_REGADR_RCVIOSTA_CPR04		0x00202A08
//#define CMD_REGADR_RCVIOSTA_CPR05		0x00202B08
//#define CMD_REGADR_RCVIOSTA_CPR06		0x00202C08
//#else
//#define CMD_REGADR_RCVIOSTA_CPR01		D_CMM_FPGA_ADDR_DMAC3_IOST		/*	0x00103308	*/
//#define CMD_REGADR_RCVIOSTA_CPR02		D_CMM_FPGA_ADDR_DMAC4_IOST		/*	0x00103408	*/
//#define CMD_REGADR_RCVIOSTA_CPR03		D_CMM_FPGA_ADDR_DMAC5_IOST		/*	0x00103508	*//*	CPRIトレース送信データCPRI01	*/
//#define CMD_REGADR_RCVIOSTA_CPR04		D_CMM_FPGA_ADDR_DMAC6_IOST		/*	0x00103608	*//*	CPRIトレース送信データCPRI02	*/
//#define CMD_REGADR_RCVIOSTA_CPR05		D_CMM_FPGA_ADDR_DMAC7_IOST		/*	0x00103708	*//*	CPRIトレース受信データCPRI01	*/
//#define CMD_REGADR_RCVIOSTA_CPR06		D_CMM_FPGA_ADDR_DMAC8_IOST		/*	0x00103808	*//*	CPRIトレース受信データCPRI02	*/
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_RCVCSW0_CPR01		0x0020270C
//#define CMD_REGADR_RCVCSW0_CPR02		0x0020280C
//#define CMD_REGADR_RCVCSW0_CPR03		0x0020290C
//#define CMD_REGADR_RCVCSW0_CPR04		0x00202A0C
//#define CMD_REGADR_RCVCSW0_CPR05		0x00202B0C
//#define CMD_REGADR_RCVCSW0_CPR06		0x00202C0C
//#else
//#define CMD_REGADR_RCVCSW0_CPR01		D_CMM_FPGA_ADDR_DMAC3_CSW0		/*	0x0010330C	*/
//#define CMD_REGADR_RCVCSW0_CPR02		D_CMM_FPGA_ADDR_DMAC4_CSW0		/*	0x0010340C	*/
//#define CMD_REGADR_RCVCSW0_CPR03		D_CMM_FPGA_ADDR_DMAC5_CSW0		/*	0x0010350C	*//*	CPRIトレース送信データCPRI01	*/
//#define CMD_REGADR_RCVCSW0_CPR04		D_CMM_FPGA_ADDR_DMAC6_CSW0		/*	0x0010360C	*//*	CPRIトレース送信データCPRI02	*/
//#define CMD_REGADR_RCVCSW0_CPR05		D_CMM_FPGA_ADDR_DMAC7_CSW0		/*	0x0010370C	*//*	CPRIトレース受信データCPRI01	*/
//#define CMD_REGADR_RCVCSW0_CPR06		D_CMM_FPGA_ADDR_DMAC8_CSW0		/*	0x0010380C	*//*	CPRIトレース受信データCPRI02	*/
//#endif
//                                                                                          
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDDES_CPR01			0x00202100
//#define CMD_REGADR_SNDDES_CPR02			0x00202200
//#define CMD_REGADR_SNDDES_CPR03			0x00202300
//#define CMD_REGADR_SNDDES_CPR04			0x00202400
//#define CMD_REGADR_SNDDES_CPR05			0x00202500
//#define CMD_REGADR_SNDDES_CPR06			0x00202600
//#else
//#define CMD_REGADR_SNDDES_CPR01			D_CMM_FPGA_ADDR_DMAC1_DSCADD	/*	0x00103100	*/
//#define CMD_REGADR_SNDDES_CPR02			D_CMM_FPGA_ADDR_DMAC2_DSCADD	/*	0x00103200	*/
//#define CMD_REGADR_SNDDES_CPR03			0x00000000
//#define CMD_REGADR_SNDDES_CPR04			0x00000000
//#define CMD_REGADR_SNDDES_CPR05			0x00000000
//#define CMD_REGADR_SNDDES_CPR06			0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDORD_CPR01			0x00202104
//#define CMD_REGADR_SNDORD_CPR02			0x00202204
//#define CMD_REGADR_SNDORD_CPR03			0x00202304
//#define CMD_REGADR_SNDORD_CPR04			0x00202404
//#define CMD_REGADR_SNDORD_CPR05			0x00202504
//#define CMD_REGADR_SNDORD_CPR06			0x00202604
//#else
//#define CMD_REGADR_SNDORD_CPR01			D_CMM_FPGA_ADDR_DMAC1_ODR		/*	0x00103104	*/
//#define CMD_REGADR_SNDORD_CPR02			D_CMM_FPGA_ADDR_DMAC2_ODR		/*	0x00103204	*/
//#define CMD_REGADR_SNDORD_CPR03			0x00000000
//#define CMD_REGADR_SNDORD_CPR04			0x00000000
//#define CMD_REGADR_SNDORD_CPR05			0x00000000
//#define CMD_REGADR_SNDORD_CPR06			0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDIOSTA_CPR01		0x00202108
//#define CMD_REGADR_SNDIOSTA_CPR02		0x00202208
//#define CMD_REGADR_SNDIOSTA_CPR03		0x00202308
//#define CMD_REGADR_SNDIOSTA_CPR04		0x00202408
//#define CMD_REGADR_SNDIOSTA_CPR05		0x00202508
//#define CMD_REGADR_SNDIOSTA_CPR06		0x00202608
//#else
//#define CMD_REGADR_SNDIOSTA_CPR01		D_CMM_FPGA_ADDR_DMAC1_IOST		/*	0x00103108	*/
//#define CMD_REGADR_SNDIOSTA_CPR02		D_CMM_FPGA_ADDR_DMAC2_IOST		/*	0x00103208	*/
//#define CMD_REGADR_SNDIOSTA_CPR03		0x00000000
//#define CMD_REGADR_SNDIOSTA_CPR04		0x00000000
//#define CMD_REGADR_SNDIOSTA_CPR05		0x00000000
//#define CMD_REGADR_SNDIOSTA_CPR06		0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDCSW0_CPR01		0x0020210C
//#define CMD_REGADR_SNDCSW0_CPR02		0x0020220C
//#define CMD_REGADR_SNDCSW0_CPR03		0x0020230C
//#define CMD_REGADR_SNDCSW0_CPR04		0x0020240C
//#define CMD_REGADR_SNDCSW0_CPR05		0x0020250C
//#define CMD_REGADR_SNDCSW0_CPR06		0x0020260C
//#else
//#define CMD_REGADR_SNDCSW0_CPR01		D_CMM_FPGA_ADDR_DMAC1_CSW0		/*	0x0010310C	*/
//#define CMD_REGADR_SNDCSW0_CPR02		D_CMM_FPGA_ADDR_DMAC2_CSW0		/*	0x0010320C	*/
//#define CMD_REGADR_SNDCSW0_CPR03		0x00000000
//#define CMD_REGADR_SNDCSW0_CPR04		0x00000000
//#define CMD_REGADR_SNDCSW0_CPR05		0x00000000
//#define CMD_REGADR_SNDCSW0_CPR06		0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDDESDATA0_CPR01	0x00202118
//#define CMD_REGADR_SNDDESDATA0_CPR02	0x00202218
//#define CMD_REGADR_SNDDESDATA0_CPR03	0x00202318
//#define CMD_REGADR_SNDDESDATA0_CPR04	0x00202418
//#define CMD_REGADR_SNDDESDATA0_CPR05	0x00202518
//#define CMD_REGADR_SNDDESDATA0_CPR06	0x00202618
//#else
//#define CMD_REGADR_SNDDESDATA0_CPR01	D_CMM_FPGA_ADDR_DMAC1_DSCDT0	/*	0x00103118	*/
//#define CMD_REGADR_SNDDESDATA0_CPR02	D_CMM_FPGA_ADDR_DMAC2_DSCDT0	/*	0x00103218	*/
//#define CMD_REGADR_SNDDESDATA0_CPR03	0x00000000
//#define CMD_REGADR_SNDDESDATA0_CPR04	0x00000000
//#define CMD_REGADR_SNDDESDATA0_CPR05	0x00000000
//#define CMD_REGADR_SNDDESDATA0_CPR06	0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDDESDATA1_CPR01	0x0020211C
//#define CMD_REGADR_SNDDESDATA1_CPR02	0x0020221C
//#define CMD_REGADR_SNDDESDATA1_CPR03	0x0020231C
//#define CMD_REGADR_SNDDESDATA1_CPR04	0x0020241C
//#define CMD_REGADR_SNDDESDATA1_CPR05	0x0020251C
//#define CMD_REGADR_SNDDESDATA1_CPR06	0x0020261C
//#else
//#define CMD_REGADR_SNDDESDATA1_CPR01	D_CMM_FPGA_ADDR_DMAC1_DSCDT1	/*	0x0010311C	*/
//#define CMD_REGADR_SNDDESDATA1_CPR02	D_CMM_FPGA_ADDR_DMAC2_DSCDT1	/*	0x0010321C	*/
//#define CMD_REGADR_SNDDESDATA1_CPR03	0x00000000
//#define CMD_REGADR_SNDDESDATA1_CPR04	0x00000000
//#define CMD_REGADR_SNDDESDATA1_CPR05	0x00000000
//#define CMD_REGADR_SNDDESDATA1_CPR06	0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDDESDATA2_CPR01	0x00202120
//#define CMD_REGADR_SNDDESDATA2_CPR02	0x00202220
//#define CMD_REGADR_SNDDESDATA2_CPR03	0x00202320
//#define CMD_REGADR_SNDDESDATA2_CPR04	0x00202420
//#define CMD_REGADR_SNDDESDATA2_CPR05	0x00202520
//#define CMD_REGADR_SNDDESDATA2_CPR06	0x00202620
//#else
//#define CMD_REGADR_SNDDESDATA2_CPR01	D_CMM_FPGA_ADDR_DMAC1_DSCDT2	/*	0x00103120	*/
//#define CMD_REGADR_SNDDESDATA2_CPR02	D_CMM_FPGA_ADDR_DMAC2_DSCDT2	/*	0x00103220	*/
//#define CMD_REGADR_SNDDESDATA2_CPR03	0x00000000
//#define CMD_REGADR_SNDDESDATA2_CPR04	0x00000000
//#define CMD_REGADR_SNDDESDATA2_CPR05	0x00000000
//#define CMD_REGADR_SNDDESDATA2_CPR06	0x00000000
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SNDDESDATA3_CPR01	0x00202124
//#define CMD_REGADR_SNDDESDATA3_CPR02	0x00202224
//#define CMD_REGADR_SNDDESDATA3_CPR03	0x00202324
//#define CMD_REGADR_SNDDESDATA3_CPR04	0x00202424
//#define CMD_REGADR_SNDDESDATA3_CPR05	0x00202524
//#define CMD_REGADR_SNDDESDATA3_CPR06	0x00202624
//#else
//#define CMD_REGADR_SNDDESDATA3_CPR01	D_CMM_FPGA_ADDR_DMAC1_DSCDT3	/*	0x00103124	*/
//#define CMD_REGADR_SNDDESDATA3_CPR02	D_CMM_FPGA_ADDR_DMAC2_DSCDT3	/*	0x00103224	*/
//#define CMD_REGADR_SNDDESDATA3_CPR03	0x00000000
//#define CMD_REGADR_SNDDESDATA3_CPR04	0x00000000
//#define CMD_REGADR_SNDDESDATA3_CPR05	0x00000000
//#define CMD_REGADR_SNDDESDATA3_CPR06	0x00000000
//#endif
//
///*	DMAC割込み表示レジスタ	*/
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_HDLCREGLOGFIL		0x00202000
//#else
//#define CMD_REGADR_HDLCREGLOGFIL		D_CMM_FPGA_ADDR_DMA_INT_LTE		/*	0x00101110	*/
//#endif
//
///*	CPRIリンク#1 T14測定値／遅延測定ステータス表示	*/
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_T14REGLOGFIL			0x002010E0
//#else
//#define CMD_REGADR_T14REGLOGFIL			D_CMM_FPGA_ADDR_T14_STATE_1
//#endif
//
///*	CPRIリンク#1 下り遅延補正値 1/8CHIP単位 CHIP単位	*/
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_DDLREGLOGFIL			0x00201024
//#else
//#define CMD_REGADR_DDLREGLOGFIL			D_CMM_FPGA_ADDR_T14_HOSEI_1_DL_LTE
//#endif
//
///*	CPRIリンク#1 上り遅延補正値 1/8CHIP単位 CHIP単位	*/
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_UDLREGLOGFIL			0x00201028
//#else
//#define CMD_REGADR_UDLREGLOGFIL			D_CMM_FPGA_ADDR_T14_HOSEI_1_UL_LTE
//#endif
//
//#if !defined(OPT_CPRI_L2HARD)
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR01	0x00007600
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR02	0x00007604
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR03	0x00007608
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR04	0x0000760C
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR05	0x00007610
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR06	0x00007614
//
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR01	0x00007640
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR02	0x00007644
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR03	0x00007648
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR04	0x0000764C
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR05	0x00007650
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR06	0x00007654
//#else
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR01	0x00000000
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR02	0x00000000
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR03	0x00000000
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR04	0x00000000
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR05	0x00000000
//#define CMD_REGADR_SENDDMASINTCOUNT_CPR06	0x00000000
//
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR01	0x00000000
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR02	0x00000000
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR03	0x00000000
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR04	0x00000000
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR05	0x00000000
//#define CMD_REGADR_SENDDMAHINTCOUNT_CPR06	0x00000000
//#endif
//
///*	CNTアドレスをHWYアドレスに変換するDMA転送用	*/
//#define CMD_REGADR_CNTDMATOP				0xC0000000
//#define CMD_REGADR_NWPDMATOP				0xB6000000
//#define CMD_ADRCHG_CNT2NWP		(CMD_REGADR_NWPDMATOP - CMD_REGADR_CNTDMATOP)
//
//
/*!
 * 共通部自身の定数定義												*
 */
/*!< buffer size*/
#define CMD_BUFF64_SIZ			64
#define CMD_BUFF256_SIZ			256
#define CMD_BUFF1024_SIZ		1024
#define CMD_BUFF4096_SIZ		4096
#define CMD_BUFF16384_SIZ		16384
#define CMD_BUFF16M_SIZ			16777216

/*!< buffer type index*/
#define CMD_BUFF64_IDX 			0
#define CMD_BUFF256_IDX 		1
#define CMD_BUFF1024_IDX 		2
#define CMD_BUFF4096_IDX 		3
#define CMD_BUFF16384_IDX 		4
#define CMD_BUFF16M_IDX 		5

/*!< max of buffer type number*/
#define CMD_BUFFTYP_MAX 		6

/*!< buffer size for each buffer type*/
#define CMD_BUFF64_NUM			4096
#define CMD_BUFF256_NUM 		2048
#define CMD_BUFF1024_NUM 		2048
#define CMD_BUFF4096_NUM 		2048
#define CMD_BUFF16384_NUM 		1024
#define CMD_BUFF16M_NUM         6

/*!<busy flag*/
#define D_TCM_BUSY_ON		1
#define D_TCM_BUSY_OFF 		0
//
///*	マスク値	*/
//#if !defined(OPT_CPRI_L2HARD)
//#define	CMD_REGMSK_CPRISTACHG		0x003F		/*	CPRIリンク#N State状変マスクON		*/
//#define	CMD_REGUNMSK_CPRISTACHG		0x003F		/*	CPRIリンク#N State状変マスクOFF		*/
//#define	CMD_REGUNMSK_CPRIENB		0x003F		/*	CPRI リンク使用/未使用 割込み要因マスクOFF	*/
//#else
//#define	CMD_REGMSK_CPRISTACHG		0x00FF		/*	CPRIリンク#N State状変マスクON		*/
//#define	CMD_REGUNMSK_CPRISTACHG		0x00FF		/*	CPRIリンク#N State状変マスクOFF		*/
//#define	CMD_REGUNMSK_CPRIENB		0x0003		/*	CPRI リンク使用/未使用 割込み要因マスクOFF	*/
//#endif
//#define	CMD_REGMSK_IRQ3_PFMSK2		0x0001		/*	IRQ3:CPRIST PRIMARY FACTOR 割込MASK		*/
//#define	CMD_REGUNMSK_IRQ3_PFMSK2	0x0010		/*	IRQ3:CPRIST PRIMARY FACTOR 割込UNMASK	*/
//#define CMD_REGCLR_CPRISTACHGDSP	0x003F		/*  CPRIリンク#1~#6 State状変表示レジスタクリア 	*/
//
///* TCP link connection/release mode */
//#define CMD_DEBUG_MODE   	D_FBNO_CPRIMT_DBG	/* Debug mode */
//#define CMD_MONITOR_MODE 	D_FBNO_CPRIMT_WAT	/* Monitor mode */
//
///*!< CPRI-MT TCP link type */
//#define D_TCM_TCPLNK_MON   			0			/*!< CPRI-MT (Monitor) TCP link */
//#define D_TCM_TCPLNK_DBG 			1			/*!< CPRI-MT (Debug) TCP link */
//
///****************************************************************************/
///*																			*/
///*								BCDコード転換								*/
///*																			*/
///****************************************************************************/
#if 0
#define D_TRP_CNVBCD(hex) (\
		((((hex)/10000000)   )<<28)+ \
		((((hex)/ 1000000)%10)<<24)+ \
		((((hex)/  100000)%10)<<20)+ \
		((((hex)/   10000)%10)<<16)+ \
		((((hex)/    1000)%10)<<12)+ \
		((((hex)/     100)%10)<< 8)+ \
		((((hex)/      10)%10)<< 4)+ \
		((((hex)         )%10)    )  \
)
#endif
/*!< size adjusting */
#define D_TCM_ADJUST f_trp_com_Fsize_adj

///*!<ERR set/rel VALUE (cpri link err )*/
//#if !defined(OPT_CPRI_L2HARD)
//#define  CMD_REG_SET_ERR_CPR_01  0x00000001
//#define  CMD_REG_SET_ERR_CPR_02  0x00000010
//#define  CMD_REG_SET_ERR_CPR_03  0x00000100
//#define  CMD_REG_SET_ERR_CPR_04  0x00001000
//#define  CMD_REG_SET_ERR_CPR_05  0x00010000
//#define  CMD_REG_SET_ERR_CPR_06  0x00100000
//#else
//#define  CMD_REG_SET_ERR_CPR_01  0x00000001
//#define  CMD_REG_SET_ERR_CPR_02  0x00010000
//#define  CMD_REG_SET_ERR_CPR_03  0x00000000
//#define  CMD_REG_SET_ERR_CPR_04  0x00000000
//#define  CMD_REG_SET_ERR_CPR_05  0x00000000
//#define  CMD_REG_SET_ERR_CPR_06  0x00000000
//#endif
//
///*!<ERR set/rel VALUE (NoActive)*/
//#define  CMD_REGSET_ERR03	D_CMM_BIT_CNT_SOFTERR03		/*	CPRI#1 NoActive	*/
//#define  CMD_REGSET_ERR04	D_CMM_BIT_CNT_SOFTERR04		/*	CPRI#2 NoActive	*/
//#define  CMD_REGSET_ERR08	D_CMM_BIT_CNT_HARDERR08		/*	CPRI#3 NoActive	*/
//#define  CMD_REGSET_ERR20	D_CMM_BIT_CNT_HARDERR20		/*	CPRI#4 NoActive	*/
//#define  CMD_REGSET_ERR63	D_CMM_BIT_CNT_HARDERR63		/*	CPRI#5 NoActive	*/
//#define  CMD_REGSET_ERR64	D_CMM_BIT_CNT_HARDERR64		/*	CPRI#6 NoActive	*/
//
///*!<ERR MASK VALUE (cpri link err )*/
//#define  CMD_REGMSK_SET_ERR_CPR  0x00777777 
//
///*!<ERR set/rel register */
//#if !defined(OPT_CPRI_L2HARD)
//#define  CMD_REGADR_ERR1_SET  0x00001024
//#define  CMD_REGADR_ERR1_REL  0x00001090
//#define  CMD_REGADR_ERR2_SET  0x00001028
//#define  CMD_REGADR_ERR2_REL  0x00001094
//#else
//#define  CMD_REGADR_ERR1_SET  0x00000000
//#define  CMD_REGADR_ERR1_REL  0x00000000
//#define  CMD_REGADR_ERR2_SET  D_CMM_FPGA_ADDR_SOFT_ERR_ON_4_2
//#define  CMD_REGADR_ERR2_REL  D_CMM_FPGA_ADDR_SOFT_ERR_OFF_4_2
//#endif
//
//#define  D_CPC_ERRREGTBL_NUM   2	/* MAX index of ERR register table*/
//
//#if defined(OPT_CPRI_L2HARD)
//#define  CMD_REGADR_CPRI_1_3G_CARRIER	0x282230
//#define  CMD_REGADR_CPRI_2_3G_CARRIER	0x282240
//
//#define  CMD_CPRI_3G_CARRIER_EN			0x80000000
//#define  CMD_CPRI_3G_CARRIER_MAXVAL		4
//#endif
//
///****************************************************************************/
//
///*!<CPRIMUX register address define */
//#define	D_TCM_CPRIMUX_REGADR_LK1_CPRI_USE		0xA0		/* LINK#1　CPRIリンク有効／無効 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_BAND			0xA1		/* LINK#1　帯域情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_3G_CA			0xA3		/* LINK#1　3Gキャリア#X運用有無 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC1_FSET1	0xA4		/* LINK#1　3Gキャリア#1下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC1_FSET2	0xA5		/* LINK#1　3Gキャリア#1下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC2_FSET1	0xA6		/* LINK#1　3Gキャリア#2下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC2_FSET2	0xA7		/* LINK#1　3Gキャリア#2下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC3_FSET1	0xA8		/* LINK#1　3Gキャリア#3下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC3_FSET2	0xA9		/* LINK#1　3Gキャリア#3下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC4_FSET1	0xAA		/* LINK#1　3Gキャリア#4下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_D3GC4_FSET2	0xAB		/* LINK#1　3Gキャリア#4下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC1_FSET1	0xAC		/* LINK#1　3Gキャリア#1上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC1_FSET2	0xAD		/* LINK#1　3Gキャリア#1上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC2_FSET1	0xAE		/* LINK#1　3Gキャリア#2上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC2_FSET2	0xAF		/* LINK#1　3Gキャリア#2上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC3_FSET1	0xB0		/* LINK#1　3Gキャリア#3上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC3_FSET2	0xB1		/* LINK#1　3Gキャリア#3上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC4_FSET1	0xB2		/* LINK#1　3Gキャリア#4上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_U3GC4_FSET2	0xB3		/* LINK#1　3Gキャリア#4上り周波数情報設定 */
//#define D_TCM_CPRIMUX_REGADR_LK1_REHDLC_RATE	0xB4		/* LINK#1　REリンクHDLCレート情報設定 */
//
//#define	D_TCM_CPRIMUX_REGADR_LK2_CPRI_USE		0xC0		/* LINK#2　CPRIリンク有効／無効 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_BAND			0xC1		/* LINK#2　帯域情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_3G_CA			0xC3		/* LINK#2　3Gキャリア#X運用有無 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC1_FSET1	0xC4		/* LINK#2　3Gキャリア#1下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC1_FSET2	0xC5		/* LINK#2　3Gキャリア#1下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC2_FSET1	0xC6		/* LINK#2　3Gキャリア#2下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC2_FSET2	0xC7		/* LINK#2　3Gキャリア#2下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC3_FSET1	0xC8		/* LINK#2　3Gキャリア#3下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC3_FSET2	0xC9		/* LINK#2　3Gキャリア#3下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC4_FSET1	0xCA		/* LINK#2　3Gキャリア#4下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_D3GC4_FSET2	0xCB		/* LINK#2　3Gキャリア#4下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC1_FSET1	0xCC		/* LINK#2　3Gキャリア#1上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC1_FSET2	0xCD		/* LINK#2　3Gキャリア#1上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC2_FSET1	0xCE		/* LINK#2　3Gキャリア#2上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC2_FSET2	0xCF		/* LINK#2　3Gキャリア#2上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC3_FSET1	0xD0		/* LINK#2　3Gキャリア#3上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC3_FSET2	0xD1		/* LINK#2　3Gキャリア#3上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC4_FSET1	0xD2		/* LINK#2　3Gキャリア#4上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_U3GC4_FSET2	0xD3		/* LINK#2　3Gキャリア#4上り周波数情報設定 */
//#define D_TCM_CPRIMUX_REGADR_LK2_REHDLC_RATE	0xD4		/* LINK#2　REリンクHDLCレート情報設定 */
//
//#define	D_TCM_CPRIMUX_REGADR_LK3_CPRI_USE		0xE0		/* LINK#3　CPRIリンク有効／無効 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_BAND			0xE1		/* LINK#3　帯域情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_3G_CA			0xE3		/* LINK#3　3Gキャリア#X運用有無*/
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC1_FSET1	0xE4		/* LINK#3　3Gキャリア#1下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC1_FSET2	0xE5		/* LINK#3　3Gキャリア#1下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC2_FSET1	0xE6		/* LINK#3　3Gキャリア#2下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC2_FSET2	0xE7		/* LINK#3　3Gキャリア#2下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC3_FSET1	0xE8		/* LINK#3　3Gキャリア#3下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC3_FSET2	0xE9		/* LINK#3　3Gキャリア#3下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC4_FSET1	0xEA		/* LINK#3　3Gキャリア#4下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_D3GC4_FSET2	0xEB		/* LINK#3　3Gキャリア#4下り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC1_FSET1	0xEC		/* LINK#3　3Gキャリア#1上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC1_FSET2	0xED		/* LINK#3　3Gキャリア#1上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC2_FSET1	0xEE		/* LINK#3　3Gキャリア#2上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC2_FSET2	0xEF		/* LINK#3　3Gキャリア#2上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC3_FSET1	0xF0		/* LINK#3　3Gキャリア#3上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC3_FSET2	0xF1		/* LINK#3　3Gキャリア#3上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC4_FSET1	0xF2		/* LINK#3　3Gキャリア#4上り周波数情報設定 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_U3GC4_FSET2	0xF3		/* LINK#3　3Gキャリア#4上り周波数情報設定 */
//#define D_TCM_CPRIMUX_REGADR_LK3_REHDLC_RATE	0xF4		/* LINK#3　REリンクHDLCレート情報設定 */
//
//#define	D_TCM_CPRIMUX_REGADR_CPRI_L1_TIM	0x70	/* L1 start-up timer */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_N1		0x71	/* LAPB 最大フレーム情報部バイト数（N1) */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_N2		0x72	/* LAPB システム回復以降タイムアウト回数(N2) */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_K			0x73	/* LAPB 最大アウトスタンディングIフレーム数 */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_T1		0x74	/* LAPB 応答確認用タイマ(T1) */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_T2		0x75	/* LAPB 応答送信用タイマ(T2) */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_T3		0x76	/* LAPB 相手局ビジー監視タイマ(T3) */
//#define	D_TCM_CPRIMUX_REGADR_LAPB_T4		0x77	/* LAPB 自局相手局アイドル状態監視タイマ(T4) */
//#define	D_TCM_CPRIMUX_REGADR_LK1_CPRI_S3_T1	0x0078	/* LINK#1 CPRI安定化時間パラメータ(S3G) */
//#define	D_TCM_CPRIMUX_REGADR_LK2_CPRI_S3_T1	0x4078	/* LINK#2 CPRI安定化時間パラメータ(S3G) */
//#define	D_TCM_CPRIMUX_REGADR_LK3_CPRI_S3_T1	0x8078	/* LINK#3 CPRI安定化時間パラメータ(S3G) */
//#define	D_TCM_CPRIMUX_REGADR_LK1_CPRI_3G_T1	0x0079	/* LINK#1 CPRI安定化時間パラメータ(3G) */
//#define	D_TCM_CPRIMUX_REGADR_LK2_CPRI_3G_T1	0x4079	/* LINK#2 CPRI安定化時間パラメータ(3G) */
//#define	D_TCM_CPRIMUX_REGADR_LK3_CPRI_3G_T1	0x8079	/* LINK#3 CPRI安定化時間パラメータ(3G) */
//#define	D_TCM_CPRIMUX_REGADR_CPRI_RE_T1		0x7A	/* CPRI安定化時間パラメータ(RE) */
//#define	D_TCM_CPRIMUX_REGADR_L1RST_T1		0x7B	/* RE 強制リセット確認タイマ */
//#define	D_TCM_CPRIMUX_REGADR_L1RST_RETRY	0x7C	/* RE 強制リセット再送回数 */
//#define	D_TCM_CPRIMUX_REGADR_L1RST_AV		0x7E	/* RE 強制リセット送信タイムアウト */
//
//#if !defined(OPT_CPRI_L2HARD)
//#define	D_TCM_CPRIMUX_REGADR_LK1_DLRE_032	0x1116	/* LINK#1 RE 送信フレーム位相調整値設定 1/32chip単位 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_DLRE_032	0x5116	/* LINK#2 RE 送信フレーム位相調整値設定 1/32chip単位 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_DLRE_032	0x9116	/* LINK#3 RE 送信フレーム位相調整値設定 1/32chip単位 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_DLRE_640	0x1117	/* LINK#1 RE 送信フレーム位相調整値設定 1/640chip単位 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_DLRE_640	0x5117	/* LINK#2 RE 送信フレーム位相調整値設定 1/640chip単位 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_DLRE_640	0x9117	/* LINK#3 RE 送信フレーム位相調整値設定 1/640chip単位 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_DLRE_FLG	0x1118	/* LINK#1 RE 送信フレーム位相調整値設定有効フラグ */
//#define	D_TCM_CPRIMUX_REGADR_LK2_DLRE_FLG	0x5118	/* LINK#2 RE 送信フレーム位相調整値設定有効フラグ */
//#define	D_TCM_CPRIMUX_REGADR_LK3_DLRE_FLG	0x9118	/* LINK#3 RE 送信フレーム位相調整値設定有効フラグ */
//#endif
//
///* CPRIMUXローカルレジスタ間接アクセス制御 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_LCR_CTRL	0xBD	/* LINK#1　ローカルレジスタ間接アクセス制御 */
//#define	D_TCM_CPRIMUX_REGADR_LK1_LCR_ADR	0xBE	/* LINK#1　ローカルレジスタ間接アクセスアドレス */
//#define	D_TCM_CPRIMUX_REGADR_LK1_LCR_DATA	0xBF	/* LINK#1　ローカルレジスタ間接アクセスデータ */
//#define	D_TCM_CPRIMUX_REGADR_LK2_LCR_CTRL	0xDD	/* LINK#2　ローカルレジスタ間接アクセス制御 */
//#define	D_TCM_CPRIMUX_REGADR_LK2_LCR_ADR	0xDE	/* LINK#2　ローカルレジスタ間接アクセスアドレス */
//#define	D_TCM_CPRIMUX_REGADR_LK2_LCR_DATA	0xDF	/* LINK#2　ローカルレジスタ間接アクセスデータ */
//#define	D_TCM_CPRIMUX_REGADR_LK3_LCR_CTRL	0xFD	/* LINK#3　ローカルレジスタ間接アクセス制御 */
//#define	D_TCM_CPRIMUX_REGADR_LK3_LCR_ADR	0xFE	/* LINK#3　ローカルレジスタ間接アクセスアドレス */
//#define	D_TCM_CPRIMUX_REGADR_LK3_LCR_DATA	0xFF	/* LINK#3　ローカルレジスタ間接アクセスデータ */
//
//
//#define	D_TCM_CPRIMUX_REGADR_LOGSET			0x0410
//#define	D_TCM_CPRIMUX_REGADR_LOGSET_OFFSET	0x4000
//
//#define D_TCM_MSK_LCR_CTRL_BSY_BIT	0x0100			/* BSY bit mask value of LINK#x　ローカルレジスタ間接アクセス制御	*/
//#define	D_TCM_MSK_LCR_CTRL_RW_BIT_ON	0x0001		/* R_W bit ON mask value of LINK#x　ローカルレジスタ間接アクセス制御 */
//#define	D_TCM_MSK_LCR_CTRL_RW_BIT_OFF	0xFFF0		/* R_W bit OFF mask value of LINK#x　ローカルレジスタ間接アクセス制御 */
//
///* CNT BUS related register address define */
//#define	D_TCM_REGADR_CNTBUS_ADD		0x00001800		/* 制御用アドレス設定 */
//#define	D_TCM_REGADR_CNTBUS_DATA	0x00001804		/* 制御用データ設定 */
//#define	D_TCM_REGADR_CNTBUS_CNT		0x00001808		/* 制御用制御レジスタ */
//
//
///* 	リセット要因表示レジスタ	*/
//#define	D_TCM_REGADR_RSTVIEW		0x00000034		/*!< リセット要因表示レジスタ */
//
//#define D_TCM_RGV_RSTVIEW_LSYSRST_C		0x80000000
//#define D_TCM_RGV_RSTVIEW_RSYSRST		0x20000000
//#define D_TCM_RGV_RSTVIEW_MSYSRST_C		0x10000000
//
//#define D_TCM_RGV_RSTVIEW_LOCALRST		0x00008000
//#define D_TCM_RGV_RSTVIEW_REMOTERST		0x00004000
//#define D_TCM_RGV_RSTVIEW_SELFRST		0x00002000
//
//#define D_TCM_CMUXRSTCHK_MSK			(D_TCM_RGV_RSTVIEW_LSYSRST_C | D_TCM_RGV_RSTVIEW_RSYSRST | D_TCM_RGV_RSTVIEW_MSYSRST_C)
//#define D_TCM_CARDRSTCHK_MSK			(D_TCM_RGV_RSTVIEW_LOCALRST | D_TCM_RGV_RSTVIEW_REMOTERST | D_TCM_RGV_RSTVIEW_SELFRST)
//
//
//#define	D_TCM_MSK_SDT_ST_BIT		0x80000000		/* SDT_ST bit mask value */ 
//#define	D_TCM_REGVAL_CNTBUS_W		0x80000000		/* set value for register write */
//#define	D_TCM_REGVAL_CNTBUS_R		0xC0000000		/* set value for register read */ 
//
//#define	D_TCM_CARDID_CPRIMUX1		0xC				/* CARD ID of CPRIMUX1 */
//#define	D_TCM_CARDID_CPRIMUX2		0xD				/* CARD ID of CPRIMUX2 */

/*TBD FHM value???*/

#if !defined(OPT_CPRI_L2HARD)
/* CPRIリンク 障害要因ステータス レジスタ */
#define	D_TCM_REGADR_CPRI1_ERR_STATE	0x0000130C	/* CPRIリンク#1障害要因ステータス レジスタ */
#else
#define	D_TCM_REGADR_CPRI1_ERR_STATE	0x00282008	/* CPRIリンク#1 障害表示(LTE) レジスタ */
#endif

#define D_TCM_REG_CPRI_ADRADD			0x10

#if !defined(OPT_CPRI_L2HARD)
/* CPRIリンク 対向フレーマ障害表示レジスタ */
#define	D_TCM_REGADR_CPRI1_RFLM_ERR		0x00201040	/* CPRIリンク#1対向フレーマ障害表示 レジスタ */
#else
#define	D_TCM_REGADR_CPRI1_RFLM_ERR		0x00282004	/* CPRIリンク#1 対向フレーマ障害表示(LTE) レジスタ */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define	D_TCM_REG_CPRI_ADRADD_R			0x100
#else
#define	D_TCM_REG_CPRI_ADRADD_R			0x10
#endif


/** @name	Inter-Thread Semaphore ID for TRIF */
/* @{ */
//#define D_TCM_SEM_ACTCPRINO			0			/* Active CPRI link no		*/
//#define D_TCM_SEM_CPRISTA			1			/* newest CPRI link state	*/
//#define D_TCM_SEM_ALMLOGTBL			2			/* Alarm log table regist	*/
/* @} */

/** @name	System define for TRIF */
/* @{ */
#define CMD_SYS_3G					0			/* System 3G  */
#define CMD_SYS_S3G					1			/* System S3G */
/* @} */

/** @name	2nd reset time vanlue for TRIF */
/* @{ */
//#define D_TCM_2NDRST_TIMVAL 		(8)		/* RE2次リセット完了待ちタイマ */
//#define D_TCM_2NDRST_TIMSTP 		(0)		/* RE2次リセットタイマ無効     */
/* @} */

/** @name	Default system parameter */
/* @{ */
#define D_TCM_SYSDFT_SCTSET_FLAG		(1) 	/* Sector use flag  */
//#define D_TCM_SYSDFT_REC_RE_DELAY		(97) 	/* REC and RE max delay (unit:1 chip) */
//#define D_TCM_SYSDFT_DDL_BDE			(12)  	/* BDE down-link delay */  
//#define D_TCM_SYSDFT_UDL_BDE			(12)  	/* BDE up-link delay */  
//#define D_TCM_SYSDFT_DDL__BB			(128)  	/* BB down-link delay */  
//#define D_TCM_SYSDFT_UDL__BB			(100)  	/* BB up-link delay */  
//#define D_TCM_SYSDFT_CPRI01_USEFLG		(1)  	/* CPRI link#01 use flag */
//#define D_TCM_SYSDFT_CPRI02_USEFLG		(1)  	/* CPRI link#02 use flag */
//#define D_TCM_SYSDFT_CPRI03_USEFLG		(1)  	/* CPRI link#03 use flag */
//#define D_TCM_SYSDFT_CPRI04_USEFLG		(1)  	/* CPRI link#04 use flag */
//#define D_TCM_SYSDFT_CPRI05_USEFLG		(1)  	/* CPRI link#05 use flag */
//#define D_TCM_SYSDFT_CPRI06_USEFLG		(1)  	/* CPRI link#06 use flag */
//#define D_TCM_SYSDFT_CPRI01_RETYP		(1)  	/* CPRI link#01 connected RE type */
//#define D_TCM_SYSDFT_CPRI02_RETYP		(1)  	/* CPRI link#02 connected RE type */
//#define D_TCM_SYSDFT_CPRI03_RETYP		(1)  	/* CPRI link#03 connected RE type */
//#define D_TCM_SYSDFT_CPRI04_RETYP		(1)  	/* CPRI link#04 connected RE type */
//#define D_TCM_SYSDFT_CPRI05_RETYP		(1)  	/* CPRI link#05 connected RE type */
//#define D_TCM_SYSDFT_CPRI06_RETYP		(1)  	/* CPRI link#06 connected RE type */
//#define D_TCM_SYSDFT_RESVMT_IP_H		(0xC0A8)  /* SV-MT (RE side) IP address (high) */
//#define D_TCM_SYSDFT_RESVMT_IP_L		(0x0181)  /* SV-MT (RE side) IP address (low) */
//#define D_TCM_SYSDFT_RESVMT_PORT		(52000)  /* SV-MT (RE side) PORT number */
//#define D_TCM_SYSDFT_REFLDMT_IP_H		(0xC0A8)  /* FLD-MT (RE side) IP address (high) */
//#define D_TCM_SYSDFT_REFLDMT_IP_L		(0x0181)  /* FLD-MT (RE side) IP address (low) */
//#define D_TCM_SYSDFT_REFLDMT_CMD_PT		(55000)  /* FLD-MT (RE side) command PORT number */
//#define D_TCM_SYSDFT_REFLDMT_FIL_PT		(55001)  /* FLD-MT (RE side) file PORT number */
//
/* (CPRI) System parameter A */

#define D_TCM_SYSDFT_REFRCRST_CNFTIM	(100) 	/* RE force reset request confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_CPRISTBTIM			(20)  /* CPRI state stable timer (REC) (unit:10ms) */

#define D_TCM_SYSDFT_RERST_CNFTIM		(100) 	/* RE reset request confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_HCRSP_ITVTIM		(100)	/* Health check response interval timer (unit:10ms) */
//#define D_TCM_SYSDFT_HCRSP_CNFTIM		(16)  	/* Health check response confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_RETIMSET_ITVTIM	(360) 	/* RE time setting interval timer (unit:10s) */
#define D_TCM_SYSDFT_SYSFILINF_CNFTIM	(100)  	/* (SYS) file INFO report confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_SYSSNDCMP_CNFTIM	(800)  	/* system parameter send complete confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_FRMFILRPT_CNFTIM	(100) 	/* firmware file report notice confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_FRMFILINFO_CNFTIM	(100) 	/* (FIRM) file INFO report confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_FRMSNDCMP_CNFTIM	(14400) /* Firmware sending complete confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_RECOMSET_CNFTIM	(100)  	/* RE common link setting confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_REEQPINF_ITVTIM	(100) 	/* RE equipment-information REQ interval timer(unit:10ms) */
#define D_TCM_SYSDFT_REEQPINF_CNFTIM	(100)  	/* RE equipment-information REQ confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_RESLTINF_CNFTIM	(20)  	/* RE slot information request confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_RESTSREQ_ITVTIM	(100) 	/* RE status report request interval timer (unit:10ms) */
#define D_TCM_SYSDFT_RESTSREQ_CNFTIM	(20) 	/* RE status report request interval timer (unit:10ms) */
#define D_TCM_SYSDFT_REPORTSTS_ITVTIM	(100) 	/* RE port status report REQ interval timer (unit:10ms) */
#define D_TCM_SYSDFT_REPORTSTS_CNFTIM	(20) 	/* RE port status report REQ confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_RECRDSTS_ITVTIM	(20) 	/* RE card status report REQ interval timer (unit:10ms) */
#define D_TCM_SYSDFT_RECRDSTS_CNFTIM	(18) 	/* RE card status report REQ confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_RECARSTS_ITVTIM	(20) 	/* RE carrier status report REQ interval timer (unit:10ms) */
#define D_TCM_SYSDFT_RECARSTS_CNFTIM	(18) 	/* RE carrier status report REQ confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_FLDMTADR_CNFTIM	(100) 	/* FLD-MT address setting confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_SVMTADR_CNFTIM		(100) 	/* SV-MT address setting confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_TOFVALSND_ITVTIM	(100) 	/* Toffset report request interval timer (unit:10ms) */
#define D_TCM_SYSDFT_TOFVAL_CNFTIM		(100)  	/* Toffset report request confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_TRXSET_CNFTIM		(18) 	/* TRX set request confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_TRXREL_CNFTIM		(18) 	/* TRX release request confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_NOMODCARRQ_CNFTIM	(19) 	/* Carrier ON/OFF request confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_RELOGGET_CNFTIM	(16) 	/* RE trouble log getting request confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_EXTDATSND_CNFTIM	(100) 	/* external device data sending confirm timer (unit: 10ms) */

#define D_TCM_SYSDFT_FLDMTREL_CNFTIM	(100)	/* FLD-MT connection force release confirm timer(unit:10ms) */
#define D_TCM_SYSDFT_FLDCMDSND_CNFTIM	(100)	/* FLD-MT command data sending confirm timer (unit:10ms)) */
#define D_TCM_SYSDFT_FLDFILSND_CNFTIM	(100)	/* FLD-MT file data sending confirm timer (unit:10ms) */

#define D_TCM_SYSDFT_SVMTREL_CNFTIM		(100)	/* FLD-MT connection force release confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_SVMTSND_CNFTIM		(100)	/* SV-MT data sending confirm timer (unit:10ms) */

/* (CPRI) System parameter C */

//#define D_TCM_SYSDFT_REFRCRST_RTNUM		(3) 	/* RE force reset retry number (unit:1 ) */
//#define D_TCM_SYSDFT_REFRCRSTSND_RTNUM	(2) 	/* RE force reset sending retry number (unit:1 ) */
//#define D_TCM_SYSDFT_HCRSP_UNCNFNUM		(5) 	/* Health check response unconfirm number (unit:1 ) */
//
//#define D_TCM_SYSDFT_SYSFILINF_RTNUM	(3)  	/* (SYS)file INFO report notice retry number (unit:1) */
//#define D_TCM_SYSDFT_SYSSND_RTNUM		(3)  	/* system parameter send retry number (unit:1) */
//#define D_TCM_SYSDFT_FRMFILRPT_RTNUM	(3)		/* firmware file report notice retry number (unit:1) */
//#define D_TCM_SYSDFT_FRMFILINF_RTNUM	(3)		/* (FIRM)file INFO report notice retry number (unit:1) */
//#define D_TCM_SYSDFT_FRMSND_RTNUM		(3)	  	/* Firmware sending retry number (unit:1) */
//
//#define D_TCM_SYSDFT_RECOMSET_RTNUM		(3)		/* RE common link setting retry number (unit:1) */
//
//#define D_TCM_SYSDFT_REEQPINF_RTNUM		(3)		/* RE equipment-information REQ retry number (unit:1) */
//#define D_TCM_SYSDFT_RESLTINF_RTNUM		(3)		/* RE slot information request retry number (unit:1) */
//#define D_TCM_SYSDFT_RESTSREQ_RTNUM		(3)		/* RE status report request retry number (unit:1) */
//#define D_TCM_SYSDFT_REPORTSTS_RTNUM	(3)		/* RE port status report REQ retry number (unit:1) */
//
//#define D_TCM_SYSDFT_RECRDSTSREQ_RTNUM	(3)		/* RE card status report request retry number (unit:1 ) */
//
//#define D_TCM_SYSDFT_RECARSTSREQ_RTNUM	(3)		/* RE card status report request retry number (unit:1 ) */
//
#define D_TCM_SYSDFT_TOFVALREQ_RTNUM	(3)  	/* Toffset report request retry number (unit:1 ) */
//#define D_TCM_SYSDFT_TRXSETREQ_RTNUM	(3)  	/* TRX set request retry number (unit:1 ) */
//#define D_TCM_SYSDFT_TRXRELREQ_RTNUM	(3)  	/* TRX release request retry number (unit:1 ) */
//#define D_TCM_SYSDFT_NOMODCARREQ_RTNUM	(3)  	/* Carrier ON/OFF specify request retry number (unit:1 ) */

#define D_TCM_SYSDFT_FLDMTADR_RTNUM		(3)  	/* FLD-MT address setting retry number (unit:1 ) */
#define D_TCM_SYSDFT_SVMTADR_RTNUM		(3)  	/* SV-MT address setting retry number (unit:1 ) */
//#define D_TCM_SYSDFT_FLDMTREL_RTNUM		(3)  	/* FLD-MT connection force release retry number (unit:1 ) */
//#define D_TCM_SYSDFT_SVMTREL_RTNUM		(3)  	/* FLD-MT connection force release retry number (unit:1 ) */
//
///* (CPRI) System parameter D */
//
//#define D_TCM_SYSDFT_D01_CPRISTBTIM		(20)	/* 01: CPRI state stable timer (RE) */
//#define D_TCM_SYSDFT_D02_HCREQCNFTIM	(120)	/* 02: Health Check request confirm timer (RE) */
//#define D_TCM_SYSDFT_D03_HCREQUNCNFNUM	(5)		/* 03: Health Check request unconfirm times(RE) */
//#define D_TCM_SYSDFT_D04_SYSDATSNDTIM	(300)	/* 04: System parameter sending confirm timer */
//#define D_TCM_SYSDFT_D05_FRMDATSNDTIM	(28000)	/* 05: Firmware data sending confirm timer */
//#define D_TCM_SYSDFT_D06_SECCHIPRVS0	(0)		/* 06: Sector n Chip revision (#0) */
//#define D_TCM_SYSDFT_D07_SECCHIPRVS1	(0)		/* 07: Sector n Chip revision (#1) */
//#define D_TCM_SYSDFT_D08_EXTDATRCVTIM	(150)	/* 08: External data receive confirm timer */
//#define D_TCM_SYSDFT_D09_FLDMTCONTIM	(150)	/* 09: FLD-MT Connection confirm timer */
//#define D_TCM_SYSDFT_D10_FLDMTRELTIM	(150)	/* 10: FLD-MT Connection release confirm timer */
//#define D_TCM_SYSDFT_D11_FLDMTCMDTIM	(150)	/* 11: FLD-MT data command receive confirm timer */
//#define D_TCM_SYSDFT_D12_FLDMTFILTIM	(150)	/* 12: FLD-MT data file receive confirm timer */
//#define D_TCM_SYSDFT_D13_SVMTCONTIM		(150)	/* 13: SV-MT Connection confirm timer */
//#define D_TCM_SYSDFT_D14_SVMTRELTIM		(150)	/* 14: SV-MT Connection release confirm timer */
//#define D_TCM_SYSDFT_D15_SVMTDATTIM		(150)	/* 15: SV-MT Data receive confirm timer */
//#define D_TCM_SYSDFT_D16_3GDIV			(2)		/* 16: 3G Div setting */
//
///* (CPRI) system parameter E */
//#define D_TCM_SYSDFT_E01_REPRISYSINF	(0)		/* 17: RE primary system  */
//#define D_TCM_SYSDFT_E02_S3GBANDW		(4)		/* 18: S3G Bandwidth  */
//#define D_TCM_SYSDFT_E03_3GBCARFLG		(15)	/* 19: 3G carrier working flag  */
//#define D_TCM_SYSDFT_E04_TXSYSNUM		(1)		/* 20: tx system number   */
//
//#define D_TCM_SYSDFT_E03_3GBCARFLG_CRA1ON		(1)	/* 3G carrier working flag car#1 ON  */
//
///* (CPRI) system parameter F */
//#define D_TCM_SYSDFT_F01_3GCAR1WKFRQ	(9612)	/* 25: 3G carrier#1 working frequency  */
//#define D_TCM_SYSDFT_F02_3GCAR2WKFRQ	(9612)	/* 26: 3G carrier#2 working frequency  */
//#define D_TCM_SYSDFT_F03_3GCAR3WKFRQ	(9612)	/* 27: 3G carrier#3 working frequency  */
//#define D_TCM_SYSDFT_F04_3GCAR4WKFRQ	(9612)	/* 28: 3G carrier#4 working frequency  */
//#define D_TCM_SYSDFT_F05_3GMTEXTDATFLG	(1)		/* 29: 3G MT/external data corresponse flag  */
//#define D_TCM_SYSDFT_F06_SYSDATWAITIM	(6000)	/* 30: System data Transfer wait timer  */
//#define D_TCM_SYSDFT_F07_FRMSNDTIM		(56000)	/* 31: Firmware sending confirm timer (update use)  */
//
///* (CPRI) system parameter G */
//#define D_TCM_SYSDFT_G_FRMRPT_CNFTIM	(200)	/* FRM file report confirm timer (Updated)(unit:10ms) */
//#define D_TCM_SYSDFT_G_FRMINFO_CNFTIM	(200)	/* (FIRM)file INFO report confirm timer (Updated)(unit:10ms) */
//#define D_TCM_SYSDFT_G_FRMCMP_CNFTIM	(28800)	/* FRM send complete confirm timer (Updated)(unit:10ms) */
//
///* (CPRI) CPRI link information */
//#define D_TCM_SYSDFT_L01_ARST_FLAG		(1)		/* RE Auto reset control flag */
//#define D_TCM_SYSDFT_L01_ARST_WATTIM	(1)		/* RE Auto reset control wait time(1s) */
/* @} */

/** @name Connected RE type from system parameter */
/* @{ */
//	/* REをBRE,RRE/LRE(MLREを含む)に分類するときは本defineを使用する */
//#define D_TCM_SYS_RETYP_OFTRX		0			/* Connected RE type in system parameter is RRE/LRE	*/
//#define D_TCM_SYS_RETYP_AMP			1			/* Connected RE type in system parameter is BRE	*/
//
/* REをBRE,RRE/LRE,MLREに分類するときは本defineを使用する */
#define D_TCM_SYS_RETYP_RRELRE		0			/* Connected RE type in system parameter is RRE/LRE	*/
#define D_TCM_SYS_RETYP_BRE			1			/* Connected RE type in system parameter is BRE		*/
#define D_TCM_SYS_RETYP_MLRE		2			/* Connected RE type in system parameter is M-LRE	*/

//#define D_TCM_BRE_NUM_MIN			1
//#define D_TCM_BRE_NUM_MAX			1
//#define D_TCM_BRE_NUM				1
//#define D_TCM_RRELRE_NUM_MIN		1
//#define D_TCM_RRELRE_NUM_MAX		6
//#define D_TCM_RRELRE_NUM			6
//
/* Activeリンクグループ */
#define D_TCM_ACTIVE_LINK_GROUP_BRE		1		/* active link group.(BRE) */
#define D_TCM_ACTIVE_LINK_GROUP_RRELRE	0		/* active link group.(RRE/LRE or invalid) */
#define D_TCM_ACTIVE_LINK_GROUP_MLRE_MIN	2		/* active link group M-LRE min */
#define D_TCM_ACTIVE_LINK_GROUP_MLRE_MAX	7		/* active link group M-LRE MAX */
#define D_TCM_ACTIVE_LINK_GROUP_NUM		7		/* valid active link group.(BRE, M-LRE) */
#define D_TCM_ACTIVE_LINK_GROUP_BRE_MIN		1
#define D_TCM_ACTIVE_LINK_GROUP_BRE_MAX		1
//
//#define D_TCM_ACTLNKGRP_NUM_MIN				0
//#define D_TCM_ACTLNKGRP_NUM_MAX				7
//#define D_TCM_ACTLNKGRP_NUM					8
//
//#define D_TCM_CARD_SLOT_NUM_MIN				1
//#define D_TCM_CARD_SLOT_NUM_MAX				56
//#define D_TCM_CARD_SLOT_NUM					56
//
/* @} */

/** @name RE link HDLC RATE info */
/* @{ */
//#define D_TCM_HDLC_RATE_960			4			/* RE link HDLC RATE info is 960kbps	*/
//#define D_TCM_HDLC_RATE_1920		8			/* RE link HDLC RATE info is 1920kbps	*/
/* @} */

/**	@name CPRI Link USE/UNUSE related register
 *	@{
 */
//#if !defined(OPT_CPRI_L2HARD)
//#define D_TCM_ADR_CPRIENB_CHG		0x00200000		/*!< Register address for CPRI Link USE/UNUSE state change interruption */
//#define D_TCM_ADR_CPRIENB_NEW		0x0020000C		/*!< Register address for CPRI Link USE/UNUSE new state */
//#define D_TCM_ADR_CPRIENB_OLD		0x00200010		/*!< Register address for CPRI Link USE/UNUSE old state */
//#else
//#define D_TCM_ADR_CPRIENB_CHG		0x00000000		/*!< Register address for CPRI Link USE/UNUSE state change interruption */
//#define D_TCM_ADR_CPRIENB_NEW		D_CMM_FPGA_ADDR_CPRI_USE	/*!< Register address for CPRI Link USE/UNUSE new state *//*	0x00282200	*/
//#define D_TCM_ADR_CPRIENB_OLD		0x00000000		/*!< Register address for CPRI Link USE/UNUSE old state */
//#endif
/* @} */

/** @name  CPRIMUX カード実装新状態表示マシケ値 */
/* @{ */
//#if !defined(OPT_CPRI_L2HARD)
//#define D_TCM_ADR_MUXCRD_STS		0x00002164		/*!< カード実装新状態表示 */
//#else
//#define D_TCM_ADR_MUXCRD_STS		0x00000000		/*!< カード実装新状態表示 */
//#endif
//#define D_TCM_RGV_MUXCRD_MSK1		0x02000000		/*!< CPRIMUX#1 カード実装新状態表示マシケ */
//#define D_TCM_RGV_MUXCRD_MSK2		0x01000000		/*!< CPRIMUX#2 カード実装新状態表示マシケ */
//#define D_TCM_RGV_IFCRD_MSK1		0x00000200		/*!< CPRIIF#1 */
//#define D_TCM_RGV_IFCRD_MSK2		0x00000100		/*!< CPRIIF#2 */
/* @} */

/** @name  CPRIMUX register Access result */
/* @{ */
//#define D_TCM_ACCESS_OK		0		/*!< OK */
//#define D_TCM_SEMGET_NG		1		/*!< Semaphore Get NG */
//#define D_TCM_SEMREL_NG		2		/*!< Semaphore Release NG */
//#define D_TCM_BSYGET_NG		3		/*!< Busy check NG */
//#define D_TCM_OTRGET_NG		4		/*!< CPRUMUX local register read NG */
//#define D_TCM_OTRSET_NG		5		/*!< CPRUMUX local register write NG */
//#define D_TCM_OTHERS_NG		6		/*!< Other NG case */
//#define D_TCM_ETIME			62		/*!< CPRIMUX indirect access timer out	*/
/* @} */

/** @name  CNT ALM type/code */
/* @{ */
//#define D_TCM_ALM_TYP_HARD		0		/*!< Hard ALM */
//#define D_TCM_ALM_TYP_SOFT		1		/*!< Soft ALM */
//#define D_TCM_ALM_TYP_REMOTE	2		/*!< Remote ALM */
//#define D_TCM_ALM_COD_SOFT		0x2D	/*!< ALM code for CPRIMUX inderect access NG */
/* @} */

/** @name  Download Stop fact */
/* @{ */
#define CMD_DLSTP_NESW		0		/*!< NE Switch */
#define CMD_DLSTP_DISC		1		/*!< Link Disconnect */
#define CMD_DLSTP_TO		2		/*!< Download T.O. */
/* @} */

/** @name  error code */
/* @{ */
//#define D_TCM_ERRCODE_SVTO D_CMM_CNT_SOFTERR05 /*!< SV-bus access TO */
/* @} */

/** @name	48hour RE ALM Watch */
/* @{ */
#if !defined(OPT_CPRI_L2HARD)
#define D_TCM_DEFAULT_RSTTIM	-1
#define	D_TCM_ALM_PROTECT_TIM	D_RRH_ALM_PROTECT_TIM	/* 172800(sec) = 48(hour) */
#else
#define D_TCM_DEFAULT_RSTTIM	0
#define	D_TCM_ALM_PROTECT_TIM	86400				/* 60*60*24(sec) = 24(hour) */
#endif
#define D_TCM_ALM_PROTECT_DEBUG	600					/* 600(sec) = 10(minutes) */

#define D_TCM_SYSDFT_RECRDSTSREQ_RTNUM	(3)			/* RE card status report request retry number (unit:1 ) */
#define D_TCM_SYSDFT_RECARSTSREQ_RTNUM	(3)			/* RE card status report request retry number (unit:1 ) */
#define D_TCM_SYSDFT_REEQPINF_RTNUM		(3)			/* RE equipment-information REQ retry number (unit:1) */
#define D_TCM_SYSDFT_REFRCRST_RTNUM		(3) 		/* RE force reset retry number (unit:1 ) */
#define D_TCM_SYSDFT_REFRCRSTSND_RTNUM	(2) 		/* RE force reset sending retry number (unit:1 ) */
#define D_TCM_SYSDFT_REPORTSTS_RTNUM	(3)			/* RE port status report REQ retry number (unit:1) */
#define D_TCM_SYSDFT_RESLTINF_RTNUM		(3)			/* RE slot information request retry number (unit:1) */
#define D_TCM_SYSDFT_RESTSREQ_RTNUM		(3)			/* RE status report request retry number (unit:1) */
#define D_TCM_SYSDFT_TRXSETREQ_RTNUM	(3)  		/* TRX set request retry number (unit:1 ) */
#define D_TCM_SYSDFT_TRXRELREQ_RTNUM	(3)  		/* TRX release request retry number (unit:1 ) */
#define D_SYSPARA_PF_C14_RECRDSTSREQ_RTNUM 0
#define D_SYSPARA_PF_C15_RECARSTSREQ_RTNUM 0
#define D_SYSPARA_PF_C10_REEQPINF_RTNUM 0
#define D_SYSPARA_PF_C01_REFRCRST_RTNUM 0
#define D_SYSPARA_PF_C02_REFRCRSTSND_RTNUM 0
#define D_SYSPARA_PF_C13_REPORTSTS_RTNUM 0
#define D_SYSPARA_PF_C11_RESLTINF_RTNUM 0
#define D_SYSPARA_PF_C12_RESTSREQ_RTNUM 0
#define D_SYSPARA_PF_C17_TRXSETREQ_RTNUM 0
#define D_SYSPARA_PF_C18_TRXRELREQ_RTNUM 0
/* @} */

/** @name	L1 Log Fact */
/* @{ */
//#define D_TCM_L1LOGINDEX_LPBTX		0
//#define D_TCM_L1LOGINDEX_LPBRXSND	1
//#define D_TCM_L1LOGINDEX_LPBRXRCV	2
//#define D_TCM_L1LOGINDEX_TXS		3
//#define D_TCM_L1LOGINDEX_RXS		4
//#define D_TCM_L1LOGINDEX_MAX		5
/* @} */

/** @name LOG Write MAX */
/* @{ */
#define CMD_LOG_WRITE_INDEX_MAX			19				 /*!< Write MAX = 19 */
#define CMD_LOG_DSPLY_INDEX_MAX			18				 /*!< Index MAX = 18 */
/* @} */

#define BPF_RU_UTTM_OK 					CMD_OK

#define CMD_RLTCODE_OK          0       /* 正常                               */
#define CMD_RLTCODE_NG          1       /* 異常                               */

#define D_COM_ITCM_OK                   BPF_RU_ITCM_OK         /*!<  正常終了   */
#define D_COM_ITCM_NG                   BPF_RU_ITCM_NG         /*!<  失敗     */
#define D_COM_ITCM_EPARAM               BPF_RU_ITCM_EPARAM     /*!<  パラメータ異常    */
#define D_COM_ITCM_ENOSEMGET            BPF_RU_ITCM_ENOSEMGET  /*!<  セマフォがロックされていた  */
#define D_COM_ITCM_ENOSPACE             BPF_RU_ITCM_ENOSPACE   /*!<  送信バッファが足りない    */
#define D_COM_ITCM_ESIZEOVER            BPF_RU_ITCM_ESIZEOVER  /*!<  サイズが上限を超えている   */
#define D_COM_ITCM_ENOQNUM              BPF_RU_ITCM_ENOQNUM    /*!<  指定のキュー番号がない    */
#define D_COM_ITCM_EADDR                BPF_RU_ITCM_EADDR      /*!<  msgpが指しているアドレスがアクセス不可  */
#define D_COM_ITCM_EINTR                BPF_RU_ITCM_EINTR      /*!<  WAIT中にシグナル受信で中断した  */
#define D_COM_ITCM_ETIMEOUT             BPF_RU_ITCM_ETIMEOUT   /*!<  タイムアウトにより終了    */
#define D_COM_ITCM_EQFULL               BPF_RU_ITCM_EQFULL     /*!<  送信キューが満杯   */

/* Return code */
#define BPF_RU_MACC_OK 0
#define BPF_RU_MACC_NG 1
#define BPF_RU_MACC_EPARAM 2

#define CMD_CPRINO_MIN          1           /* CPRI#MIN                     */
#define CMD_CPRINO_MAX          16          /* CPRI#MAX                     */
#define CMD_CARSTSMAX           1           /* キャリア状態数最大値           */
#define CMD_MAKER_NUM           32          /*  メーカ固有情報数            */
#define CMD_FAN_NUM             16          /*  FAN情報数                  */
#define CMD_SLTNO_MAX           56          /* スロット#MAX                 */

//#define CMD_CARNO_MIN           1           /* キャリア番号最小数            */
//#define CMD_CARNO_MAX 4
#define CMD_LOG_WRITE_INDEX_MAX         19               /*!< Write MAX = 19 */

#define D_TCM_BRE_NUM_MIN           1
#define D_TCM_BRE_NUM_MAX           1
#define D_TCM_BRE_NUM               1
#define D_TCM_RRELRE_NUM_MIN        1
#define D_TCM_RRELRE_NUM_MAX        6
#define D_TCM_RRELRE_NUM            6

#define D_TCM_ACTIVE_LINK_GROUP_NUM     7       /* valid active link group.(BRE, M-LRE) */
#define D_TCM_ACTLNKGRP_NUM                 8

#define D_SHMID_CPU_RSTTIMTBL       91              /*!< Reset history for BDE card table       */
#define D_SHMID_CPU_IRQCNTTBL		92				/* TBD: for funtion f_trp_com_FLog_switch */
#define f_trp_com_Fmemclr(adr, size) memset(adr, 0, size)
#define	CMD_ALM_HOLIDING			2			/* ALM保持状態				*/

#endif
/** @} */
