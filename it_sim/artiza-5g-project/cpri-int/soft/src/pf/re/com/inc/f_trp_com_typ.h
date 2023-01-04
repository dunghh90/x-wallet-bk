/*!
 *	@skip	$ld:$
 *	@file	f_trp_com_typ.h
 *	@brief	TRP common define header
 *	@date	2007/03/20	FFCS)linjunhua
 *  @date   2010/08/12 CMS)Takahashi modify for M-S3G-eNBSYS-01722
 *	@date   2010/10/22	FJT)Koshida modify for eNB-004 of TS656案5対応
 *  @date   2010/11/18 FJT)Koshida modify for M-S3G-eNBSYS-02100;☆ST_ENB☆TS841項：CPRI-MUX含まない再開を実施した際に3G側にてBRE ALM検出
 *	@date	2011/05/23	FJT)Koshida modify for 11B-01-001 (eNB-004)
 *  @date   2011/11/10 FJT)K.Koshida D-MICRO-008 3002形対応(LAPB削除,H/S inf変更対応)
 *  @date   2012/01/24 FJT)K.Koshida modify for CR-HARD-00008
 *
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2007-2012
 */

#ifndef _F_TRP_COM_TYP_h
#define _F_TRP_COM_TYP_h

//#if 0
/****************************************************************************/
/* スレッド情報テーブル														*/
/****************************************************************************/
/**
 * @brief   スレッド情報テーブル要素
 */
typedef struct{
	CHAR	thr_name[CMD_NUM8];			/**< スレッド名						*/
	USHORT	threno;						/**< スレッド外部番号				*/
	USHORT	thrqno_nml;					/**< スレッドキュー番号(通常)		*/
	ULONG	thread_id;					/**< スレッドID						*/
}	T_TCM_THRINF;

/**
 * @brief   スレッド情報テーブル
 */
typedef struct{
	T_TCM_THRINF	inf[D_TCM_THRINO_MAX];  /**< 情報						*/
}	T_TCM_THRINF_TBL;

///****************************************************************************/
///* 通信スレッド情報テーブル													*/
///****************************************************************************/
//typedef struct{
//	CHAR	thr_name[CMD_NUM8];			/* スレッド名						*/
//	USHORT	ctheno;						/* スレッド外部番号					*/
//	USHORT  dmy;						/* ダミー							*/
//	ULONG	thread_id;					/* スレッドID						*/
//}	T_TCM_CTHINF;
//typedef struct{
//	T_TCM_CTHINF	inf[D_TCM_CTHINO_MAX]; /* 情報							*/
//}	T_TCM_CTHINF_TBL;
///****************************************************************************/
///* デバッグスレッド情報テーブル												*/
///****************************************************************************/
//typedef struct{
//	CHAR	thr_name[CMD_NUM8];			/* スレッド名						*/
//	UINT	dthqno;						/* スレッドキュー番号				*/
//	UINT	pri;						/* スレッド優先度					*/
//	ULONG	thread_id;					/* スレッドID						*/
//}	T_TCM_DTHINF;
//typedef struct{
//	T_TCM_DTHINF	inf[D_TCM_DTHINO_MAX]; /* 情報							*/
//}	T_TCM_DTHINF_TBL;
//
//
///****************************************************************************/
///* スレッド起動情報テーブル													*/
///****************************************************************************/
//typedef struct{
//	INT	  pol;							/* スケジューリングポリシー			*/
//	INT	  pri;							/* スレッド優先度					*/
//	VOID* fnc;							/* 起動関数							*/
//}	T_TCM_THRSUPINF;
//typedef struct{
//	T_TCM_THRSUPINF inf[D_TCM_THRINO_MAX];	/* 情報							*/
//}	T_TCM_THRSUPINF_TBL;
//
///* ======================================================================== */
///*	CPRI信号インタフェース型定義											*/
///* ======================================================================== */
///****************************************************************************/
///* MESSAGE ：ヘルスチェック要求(CMT_CPRIF_HCREQ)							*/
///* 信号種別：0x0110			方向:TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//} CMT_CPRIF_HCREQ;
//
///****************************************************************************/
///* MESSAGE ：ヘルスチェック応答(CMT_CPRIF_HCRES)							*/
///* 信号種別：0x0120		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//} CMT_CPRIF_HCRES;
//
///****************************************************************************/
///* MESSAGE ：REリセット要求(CMT_CPRIF_RERSTREQ)								*/
///* 信号種別：0x0210		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				kind;					/* 対象指定					*/
//	USHORT				recomapp;				/* RE共通部指定				*/
//}CMT_CPRIF_RERSTREQ;
//
///****************************************************************************/
///* MESSAGE ：RE時刻設定要求(CMT_CPRIF_RETIMSETREQ)							*/
///* 信号種別：0x0310		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				year;					/* 年						*/
//	USHORT				month_day;				/* 月日						*/
//	USHORT				hour;					/* (未使用+)時				*/
//	USHORT				minute_sec;				/* 分秒						*/
//}CMT_CPRIF_RETIMSETREQ;
//
///****************************************************************************/
///* MESSAGE ：RE共通部リンク確立要求(CMT_CPRIF_RELINKSTSREQ)					*/
///* 信号種別：0x1110		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				link;					/* リンク設定				*/
//}CMT_CPRIF_LINKSTSREQ;
//
///****************************************************************************/
///* MESSAGE ：RE共通部リンク確立応答(CMT_CPRIF_RELINKSTSRES)					*/
///* 信号種別：0x1120		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_LINKSTSRES;
//
///****************************************************************************/
///* MESSAGE ：ファームウェアファイル報告要求(CMT_CPRIF_FIRMFILEREQ)			*/
///* 信号種別：0x2110		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_FIRMFILEREQ;
//
///****************************************************************************/
///* MESSAGE ：ファームウェアファイル報告応答(CMT_CPRIF_FIRMFILERES)			*/
///* 信号種別：0x2120		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				makername;				/* メーカ名					*/
//	USHORT				mjr_ver;				/* メジャーバージョン		*/
//	USHORT				mir_ver;				/* マイナーバージョン		*/
//}FIRM_FILE_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	FIRM_FILE_INF		firmfile;			/* ファームウェアファイル情報群	*/
//}CMT_CPRIF_FIRMFILERES;
//
///****************************************************************************/
///* MESSAGE ：ファイル情報報告通知(CMT_CPRIF_FILEINFREQ)						*/
///* 信号種別：0x2210		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				data;					/* 対象データ				*/
//	UCHAR				filename[20];			/* ファイル名				*/
//	UINT				size;					/* トータルサイズ			*/
//}CMT_CPRIF_FILEINFREQ;
//
///****************************************************************************/
///* MESSAGE ：ファイル情報報告応答(CMT_CPRIF_FILEINFRES)						*/
///* 信号種別：0x2220		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_FILEINFRES;
//
///****************************************************************************/
///* MESSAGE ：ファイルデータ送信(CMT_CPRIF_FILDATSND)						*/
///* 信号種別：0x2300		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				sgmtno1;				/* 分割番号1				*/
//	USHORT				sgmtno2;				/* 分割番号2				*/
//	USHORT				datsize1;				/* データサイズ1			*/
//	USHORT				datsize2;				/* データサイズ2			*/
//	UCHAR				fildat[CMD_FILEDAT_MAX];/* ファイルデータ			*/
//}CMT_CPRIF_FILEDATSND;
//
///****************************************************************************/
///* MESSAGE ：ファイルデータ送信完了報告通知(CMT_CPRIF_FILSNDFINNTC)			*/
///* 信号種別：0x2310		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				tgtdat;					/* 対象データ				*/
//	USHORT				chksum;					/* チェックサム				*/
//}CMT_CPRIF_FILSNDFINNTC;
//
///****************************************************************************/
///* MESSAGE ：ファイルデータ送信完了報告応答(CMT_CPRIF_FILSNDFINRES)		*/
///* 信号種別：0x2320		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//    USHORT              primary_sys;            /* primary system */
//}CMT_CPRIF_FILSNDFINRES;
//
//
///****************************************************************************/
///* MESSAGE ：運用中ファームウェアファイル報告要求(CMT_CPRIF_UFIRMFILEREQ)			*/
///* 信号種別：0x2411		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_UFIRMFILEREQ;
//
///****************************************************************************/
///* MESSAGE ：運用中ファームウェアファイル報告応答(CMT_CPRIF_UFIRMFILERES)			*/
///* 信号種別：0x2421		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				makername;				/* メーカ名					*/
//	USHORT				cur_mjr_ver;			/* 運用中メジャーバージョン		*/
//	USHORT				cur_mir_ver;			/* 運用中マイナーバージョン		*/
//	USHORT				nes_mjr_ver;			/* 起動用メジャーバージョン		*/
//	USHORT				nes_mir_ver;			/* 起動用マイナーバージョン		*/
//}UFIRM_FILE_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	UFIRM_FILE_INF		firmfile;			/* 運用中ファームウェアファイル情報群	*/
//}CMT_CPRIF_UFIRMFILERES;
//
///****************************************************************************/
///* MESSAGE ：運用中ファイル情報報告通知(CMT_CPRIF_UFILEINFREQ)						*/
///* 信号種別：0x2511		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				data;					/* 対象データ				*/
//	UCHAR				filename[20];			/* ファイル名				*/
//	UINT				size;					/* トータルサイズ			*/
//}CMT_CPRIF_UFILEINFREQ;
//
///****************************************************************************/
///* MESSAGE ：運用中ファイル情報報告応答(CMT_CPRIF_UFILEINFRES)						*/
///* 信号種別：0x2521		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_UFILEINFRES;
//
///****************************************************************************/
///* MESSAGE ：運用中ファイルデータ送信(CMT_CPRIF_UFILEDATSND)						*/
///* 信号種別：0x2601		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				sgmtno1;				/* 分割番号1				*/
//	USHORT				sgmtno2;				/* 分割番号2				*/
//	USHORT				datsize1;				/* データサイズ1			*/
//	USHORT				datsize2;				/* データサイズ2			*/
//	UCHAR				fildat[CMD_FILEDAT_MAX];/* ファイルデータ			*/
//}CMT_CPRIF_UFILEDATSND;
//
///****************************************************************************/
///* MESSAGE ：運用中ファイルデータ送信完了報告通知(CMT_CPRIF_UFILSNDFINNTC)			*/
///* 信号種別：0x2611		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				tgtdat;					/* 対象データ				*/
//	USHORT				chksum;					/* チェックサム				*/
//}CMT_CPRIF_UFILSNDFINNTC;
//
///****************************************************************************/
///* MESSAGE ：運用中ファイルデータ送信完了報告応答(CMT_CPRIF_UFILSNDFINRES)		*/
///* 信号種別：0x2621		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_UFILSNDFINRES;
//
///****************************************************************************/
///* MESSAGE ：TRX設定要求(CMT_CPRIF_TRXSETREQ) 								*/
///* 信号種別：0x4110		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				carno;					/* キャリア番号				*/
//	USHORT				band;					/* システム帯域幅			*/
//	USHORT				frqno;					/* 周波数番号				*/
//	USHORT				tx_brc_no;				/* 送信系	*/
//}CMT_CPRIF_TRXSETREQ;
//
//
///****************************************************************************/
///* MESSAGE ：TRX設定応答(CMT_CPRIF_TRXSETRES) 								*/
///* 信号種別：0x4120		方向：MDIF->TRIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				carno;					/* キャリア番号				*/
//}CMT_CPRIF_TRXSETRES;
//
///****************************************************************************/
///* MESSAGE ：TRX解放要求(CMT_CPRIF_TRXRELREQ) 								*/
///* 信号種別：0x4210		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				carno;					/* キャリア番号				*/
//}CMT_CPRIF_TRXRELREQ;
//
//
///****************************************************************************/
///* MESSAGE ：TRX解放応答(CMT_CPRIF_TRXRELRES) 								*/
///* 信号種別：0x4220		方向：MDIF->TRIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				carno;					/* キャリア番号				*/
//}CMT_CPRIF_TRXRELRES;
//
///****************************************************************************/
///* MESSAGE ：Toffset値報告要求(CMT_CPRIF_TOFFSETINFREQ)						*/
///* 信号種別：0x4310		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_TOFFSETINFREQ;
//
///****************************************************************************/
///* MESSAGE ：Toffset値報告応答(CMT_CPRIF_TOFFSETINFRES)						*/
///* 信号種別：0x4320		方向:MDIF->TRIF										*/
///****************************************************************************/
///* Toffset値情報 */
//typedef struct
//{
//	USHORT				toffset;				/* Toffset値				*/
//	USHORT				updeleqp;				/* 上り装置内遅延値			*/
//	USHORT				downdeleqp;				/* 下り装置内遅延値			*/
//}TOFFSET_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	TOFFSET_INF			toffset_inf;			/* Toffset値情報			*/
//}CMT_CPRIF_TOFFSETINFRES;
//
///****************************************************************************/
///* MESSAGE ：無変調キャリア出力ON/OFF指定要求(CMT_CPRIF_TRXRELREQ) 			*/
///* 信号種別：0x4410		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				carno;					/* キャリア番号				*/
//	USHORT				onoff_app;				/* ON/OFF指定				*/
//	USHORT				sndpwr;					/* 送信電力					*/
//}CMT_CPRIF_CARONOFFREQ;
//
///****************************************************************************/
///* MESSAGE ：無変調キャリア出力ON/OFF指定応答(CMT_CPRIF_CARONOFFRES)		*/
///* 信号種別：0x4420		方向：MDIF->TRIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				carno;					/* キャリア番号				*/
//	USHORT				br_app;					/* 系指定					*/
//}CMT_CPRIF_CARONOFFRES;
//
///****************************************************************************/
///* MESSAGE ：RE装置構成情報報告要求(CMT_CPRIF_EQPSTRREQ)					*/
///* 信号種別：0x5110		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_EQPSTRREQ;
//
///****************************************************************************/
///* MESSAGE ：RE装置構成情報報告応答(CMT_CPRIF_EQPSTRREP)					*/
///* 信号種別：0x5120		方向:MDIF->TRIF										*/
///****************************************************************************/
///* CPRIリンク装置構成情報 */
//typedef struct
//{
//	USHORT				maker;					/* メーカ名					*/
//	USHORT				bts_no;					/* BTS形号					*/
//	USHORT				eqp_no;					/* 装置号番					*/
//	USHORT				rekind;					/* RE種別					*/
//	USHORT				model_3g;				/* 3GBTS との共用方式		*/
//	USHORT				dummy1;					/* 空き						*/
//	USHORT				power_type;				/* 電源種別					*/
//	USHORT				rcvant;					/* 受信アンテナ数			*/
//	USHORT				sndant;					/* 送信アンテナ数			*/
//	USHORT				powerunit;				/* パワークラス単位			*/
//	USHORT				powerclass_0;			/* パワークラス(0系)		*/
//	USHORT				powerclass_1;			/* パワークラス(1系)		*/
//	USHORT				powerclass_2;			/* パワークラス(2系)		*/
//	USHORT				powerclass_3;			/* パワークラス(3系)		*/
//	USHORT				dummy2[2];				/* 空き						*/
//	USHORT				frqband;				/* 対応周波数帯				*/
//	USHORT				max_bnd;				/* 最大周波数帯域幅			*/
//	USHORT				us2g_car_no;			/* キャリア数(2GHz)			*/
//	USHORT				us15g_car_no;			/* キャリア数(1.5GHz)		*/
//	USHORT				us800g_car_no;			/* キャリア数(800GHz)		*/
//	USHORT				dummy3[3];				/* 空き						*/
//	USHORT				trx_inf_type;			/* TRX-INF タイプ			*/
//	USHORT				dummy4[39];				/* 空き						*/
//}CPRIEQPSTR_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	CPRIEQPSTR_INF		cpristr_inf;			/* CPRIリンク装置構成情報	*/
//}CMT_CPRIF_EQPSTRRES;
//
///****************************************************************************/
///* MESSAGE ：REスロット情報報告要求(CMT_CPRIF_SLOTINFREQ)					*/
///* 信号種別：0x5210		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_SLOTINFREQ;
//
///****************************************************************************/
///* MESSAGE ：REスロット情報報告応答(CMT_CPRIF_SLOTINFRES)					*/
///* 信号種別：0x5220		方向:MDIF->TRIF										*/
///****************************************************************************/
///* SLOT単位の詳細情報 */
//typedef struct
//{
//	USHORT				slot_no;				/* SLOT番号					*/
//	USHORT				card_inf;				/* REカード機能部情報		*/
//}SLOT_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
///* =========================== 可変長情報 ================================= */
///* ※応答結果以降は以下の可変長情報が付加される。							*/
///* ■パラメータ		■サイズ	■値		■説明							*/
///* ・スロット情報N	2Byte		0～8		該当CPRIリンク配下のスロット数	*/
///* ・スロット番号#1	2Byte		1～56		REのスロット番号				*/
///* ・REカード機能部															*/
///* 			 情報#1	2Byte		-			スロット番号#1のREカード機能部	*/
///*																	  情報	*/
///* 	～	（スロット情報N個分スロット番号、REカード機能部情報有効）			*/
///* ・スロット番号#n	2Byte		1～56		REのスロット番号				*/
///* ・REカード機能部															*/
///* 			 情報#n	2Byte		-			スロット番号#nのREカード機能部	*/
///* 																	  情報	*/
///* ======================================================================== */
//}CMT_CPRIF_SLOTINFRES;
//
///****************************************************************************/
///* MESSAGE ：RE状態報告要求(CMT_CPRIF_RESTSREQ)								*/
///* 信号種別：0x5310		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_RESTSREQ;
//
///****************************************************************************/
///* MESSAGE ：RE状態報告応答(CMT_CPRIF_RESTSRES)								*/
///* 信号種別：0x5320		方向:MDIF->TRIF										*/
///****************************************************************************/
///* RE状態報告詳細 */
//typedef struct
//{
//	USHORT				maker_name;				/* メーカ名					*/
///* =========================== 可変長情報 ================================= */
///* ※メーカ名以降は以下の可変長情報が付加される。							*/
///* ■パラメータ		■サイズ	■値		■説明							*/
///* ・FAN情報数N		2Byte		0～			FAN ALM情報の数					*/
///* ・FAN情報#1		2Byte		1～			FAN ALM箇所を通知する情報		*/
///* 	～	(FAN情報数N個分FAN情報有効)											*/
///* ・FAN情報#n		2Byte		1～			FAN ALM箇所を通知する情報		*/
///*																			*/
///* ・メーカ固有RE															*/
///*			情報数N	2Byte		0～			メーカ固有のREに関するALM情報数	*/
///* ・メーカ固有RE															*/
///*			情報#1	2Byte		1～			メーカ固有のREに関するALM情報	*/
///* 	～	(メーカ固有RE情報数N個分メーカ固有RE情報有効)						*/
///* ・メーカ固有RE															*/
///*			情報#n	2Byte		1～			メーカ固有のREに関するALM情報	*/
///* ======================================================================== */
//}RESTS_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	RESTS_INF			rests_inf;				/* RE状態報告詳細			*/
//}CMT_CPRIF_RESTSRES;
//
///****************************************************************************/
///* MESSAGE ：REカード情報報告要求(CMT_CPRIF_CARDINFREQ)						*/
///* 信号種別：0x5410		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				slot_no;				/* スロット番号				*/
//}CMT_CPRIF_CARDINFREQ;
//
///****************************************************************************/
///* MESSAGE ：REカード情報報告応答(CMT_CPRIF_CARDINFRES)						*/
///* 信号種別：0x5420		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				slot_no;				/* スロット番号				*/
//	UCHAR				meker_name[16];			/* メーカ名					*/
//	UCHAR				card_name[8];			/* カード名					*/
//	USHORT				up_resrc_num1;			/* 上りリソース数1			*/
//	USHORT				up_resrc_num2;			/* 上りリソース数2			*/
//	USHORT				down_resrc_num1;		/* 下りリソース数1			*/
//	USHORT				down_resrc_num2;		/* 下りリソース数2			*/
//	USHORT				card_svitem;			/* カード基本監視項目		*/
//	USHORT				card_extitem;			/* カード拡張監視項目		*/
//	USHORT				card_cntitem;			/* カード制御項目			*/
//	USHORT				funcitem;				/* 機能部固有制御項目		*/
//}CMT_CPRIF_CARDINFRES;
//
///****************************************************************************/
///* MESSAGE ：REバージョン報告要求(CMT_CPRIF_REVERREQ) 						*/
///* 信号種別：0x5510		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				tgtkind;				/* 対象指定					*/
//	USHORT				slot_no;				/* スロット番号				*/
//	USHORT				id_kind;				/* ID情報識別				*/
//}CMT_CPRIF_REVERREQ;
//#endif /* #if 0 */
//
///****************************************************************************/
///* MESSAGE ：REバージョン報告応答(CMT_CPRIF_REVERRES) 						*/
///* 信号種別：0x5520		方向:MDIF->TRIF										*/
///****************************************************************************/
///* REバージョン情報詳細 */
//typedef struct
//{
//	UCHAR				name[10];				/* 名称						*/
//	USHORT				mjr_ver;				/* メジャーバージョン		*/
//	USHORT				mir_ver;				/* マイナーバージョン		*/
//	USHORT				year;					/* 年						*/
//	USHORT				month_day;				/* 月日						*/
//}VER_INF;
//
////TODO m_cm_typ.hと競合
//#define REVER_INF REVER_INF_TRIF
///* REバージョン報告詳細 */
//typedef struct
//{
//	UCHAR				maker_name[16];			/* メーカ名					*/
//	USHORT				kind;					/* 対象指定					*/
//	USHORT				slot_no;				/* スロット番号				*/
//	UCHAR				id_kind[1024];			/* ID情報識別				*/
//	USHORT				inf_num;				/* 情報数					*/
//}REVER_INF;
//
////TODO m_cm_typ.hと競合
//#define CMT_CPRIF_REVERRES CMT_CPRIF_REVERRES_TRIF
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	REVER_INF			rever_inf;				/* REバージョン報告詳細		*/
//}CMT_CPRIF_REVERRES;
//
//#if 0
///****************************************************************************/
///* MESSAGE ：REカード制御要求(CMT_CPRIF_RECARDCNTREQ)						*/
///* 信号種別：0x5610		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				slot_no;				/* スロット番号				*/
//	USHORT				cnt_kind;				/* 制御						*/
//}CMT_CPRIF_RECARDCNTREQ;
//
//
///****************************************************************************/
///* MESSAGE ：REカード制御応答(CMT_CPRIF_RECARDCNTRES)						*/
///* 信号種別：0x5620		方向：MDIF->TRIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				slot_no;				/* スロット番号				*/
//}CMT_CPRIF_RECARDCNTRES;
//
///****************************************************************************/
///* MESSAGE ：REカード状態報告要求(CMT_CPRIF_CARDSTSREQ)						*/
///* 信号種別：0x5710		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				slot_no;				/* スロット番号				*/
//}CMT_CPRIF_CARDSTSREQ;
//
///****************************************************************************/
///* MESSAGE ：REカード状態報告応答(CMT_CPRIF_CARDSTSRES)						*/
///* 信号種別：0x5720		方向:MDIF->TRIF										*/
///****************************************************************************/
///* カード情報 */
//typedef struct
//{
//	USHORT				slot_no;				/* スロット番号				*/
//	UCHAR				card_name[8];			/* カード名称				*/
//	USHORT				year;					/* 年						*/
//	USHORT				month_day;				/* 月日						*/
//	USHORT				hour;					/* 時						*/
//	USHORT				minsec;					/* 分秒						*/
//	USHORT				card_svitem;			/* カード基本監視項目		*/
//	USHORT				card_extsvitem;			/* カード拡張監視項目		*/
//	USHORT				card_cntitem;			/* カード基本制御項目		*/
//	USHORT				card_extcntitem;		/* カード拡張制御項目		*/
//	USHORT				fun_num;				/* 機能部数					*/
///* =========================== 可変長情報 ================================= */
///* ※キャリア情報以降は以下の可変長情報が付加される。						*/
///* ■パラメータ		■サイズ	■値		■説明							*/
///* 種別				2Byte		0～28		機能部#N機能部種別				*/
///* 故障状態			2Byte		0～3		機能部#N機能部故障状態			*/
///* 基本監視項目		2Byte													*/
///* 拡張監視項目		2Byte													*/
///* 対象セクタ数		2Byte		1～6										*/
///* セクタ番号		2Byte		1～6										*/
///* キャリア番号		2Byte		1											*/
///* ・ALM情報数N		2Byte		0～255		発生しているALMの種類数			*/
///* ・ALM情報#1		2Byte		1～255		各ALMコード						*/
///* 	～	(ALM情報数N個分ALM情報有効)											*/
///* ・ALM情報#n		2Byte		1～255		各ALMコード						*/
///*																			*/
///* ・ERR情報数N		2Byte		0～255		発生しているERRの種類数			*/
///* ・ERR情報#1		2Byte		1～255		各ERRコード						*/
///* 	～	(ERR情報数N個分ERR情報有効)											*/
///* ・ERR情報#n		2Byte		1～255		各ERRコード						*/
///* ======================================================================== */
//}CARDSTS_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	CARDSTS_INF			cardsts_inf;			/* カード状態詳細			*/
//}CMT_CPRIF_CARDSTSRES;
//
///****************************************************************************/
///* MESSAGE ：REキャリア状態報告要求(CMT_CPRIF_CARLSTSREQ)					*/
///* 信号種別：0x5810		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_CARLSTSREQ;
//
///****************************************************************************/
///* MESSAGE ：REキャリア状態報告応答(CMT_CPRIF_CARLSTSRES)					*/
///* 信号種別：0x5820		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				car_num;				/* キャリア数				*/
//	USHORT				carstsinf;				/* キャリア状態詳細先頭情報	*/
///* =========================== キャリア状態詳細 =========================== */
///* ※TRX設定済みのキャリア数分、以下の情報がキャリア状態詳細先頭情報		*/
///*	 (1つ目のキャリア番号に該当)から付加される								*/
///* ■パラメータ		■サイズ	■値		■説明							*/
///* ・キャリア番号	2Byte		1～4		TRX設定済みのキャリア番号		*/
///* ・キャリア状態	2Byte		-			TRX設定済みキャリアの状態		*/
///* ・キャリア構成状態	2Byte	0～2		送信可否情報					*/
///*																			*/
///* ※キャリア構成状態以降は以下の可変長情報が付加される。					*/
///* ■パラメータ		■サイズ	■値		■説明							*/
///* ・ALM情報数N		2Byte		0～255		発生しているALMの種類数			*/
///* ・ALM情報#1		2Byte		221～255	各ALMコード						*/
///* 	～	(ALM情報数N個分ALM情報有効)											*/
///* ・ALM情報#n		2Byte		221～255	各ALMコード						*/
///*																			*/
///* ・ERR情報数N		2Byte		0～255		発生しているERR種類数			*/
///* ・ERR情報#1		2Byte		221～255	各ERRコード						*/
///* 	～	(ERR情報数N個分ERR情報有効)											*/
///* ・ERR情報#n		2Byte		221～255	各ERRコード						*/
///* 																			*/
///* ・カード数		2Byte		0～8		該当キャリアが割り当てられている*/
///* 											カード数						*/
///* ・スロット番号#1	2Byte		1～56		REのスロット番号				*/
///* 	～	（カード数分スロット番号有効）										*/
///* ・スロット番号#n	2Byte		1～56		REのスロット番号				*/
///* ======================================================================== */
//}CMT_CPRIF_CARLSTSRES;
//
///****************************************************************************/
///* MESSAGE ：RE PORT制御要求(CMT_CPRIF_REPORTCNTREQ)						*/
///* 信号種別：0x5910		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				infbit;					/* 情報ビット				*/
//	USHORT				control;				/* 制御						*/
//}CMT_CPRIF_REPORTCNTREQ;
//
///****************************************************************************/
///* MESSAGE ：RE PORT制御応答(CMT_CPRIF_REPORTCNTRES)						*/
///* 信号種別：0x5920		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				portout_sts;			/* PORT出力状態				*/
//}CMT_CPRIF_REPORTCNTRES;
//
///****************************************************************************/
///* MESSAGE ：RE PORT状態報告要求(CMT_CPRIF_REPORTSTSREQ)					*/
///* 信号種別：0x5A10		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_REPORTSTSREQ;
//
///****************************************************************************/
///* MESSAGE ：RE PORT状態報告応答(CMT_CPRIF_REPORTSTSRES)					*/
///* 信号種別：0x5A20		方向:MDIF->TRIF										*/
///****************************************************************************/
///* PORT状態詳細 */
//typedef struct
//{
//	USHORT				port_out_sts;			/* PORT出力状態				*/
//	USHORT				port_in_sts;			/* PORT入力状態				*/
//}PORTSTS_INF;
//
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	PORTSTS_INF			portsts_inf;			/* PORT状態詳細				*/
//}CMT_CPRIF_REPORTSTSRES;
//
///****************************************************************************/
///* MESSAGE ：REカード診断要求(CMT_CPRIF_RECRDDIAREQ)						*/
///* 信号種別：0x5B10		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				slot_no;				/* スロット番号				*/
//
//} CMT_CPRIF_RECRDDIAREQ;
//
///****************************************************************************/
///* MESSAGE ：REカード診断応答(CMT_CPRIF_RECRDDIARES)						*/
///* 信号種別：0x5B20		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				slot_no;				/* スロット番号				*/
//	USHORT				dia_result;				/* 診断結果					*/
//} CMT_CPRIF_RECRDDIARES;
//
///****************************************************************************/
///* MESSAGE ：RE障害ログ取得要求(CMT_CPRIF_REOBSLOGREQ)						*/
///* 信号種別：0x5C10		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				comsec;					/* RE共通部/REセクタ部指定	*/
//	UCHAR				filename[256];			/* 障害ログファイル名		*/
//}CMT_CPRIF_REOBSLOGREQ;
//
///****************************************************************************/
///* MESSAGE ：RE障害ログ取得応答(CMT_CPRIF_REOBSLOGRES)						*/
///* 信号種別：0x5C20		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	UCHAR				filename[256];			/* ファイル名				*/
//	UINT				datsize;				/* トータルデータサイズ		*/
//}CMT_CPRIF_REOBSLOGRES;
//
///****************************************************************************/
///* MESSAGE ：RE障害ログ取得中止要求(CMT_CPRIF_REOBLOGSTPREQ)				*/
///* 信号種別：0x5D10		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_REOBLOGSTPREQ;
//
///****************************************************************************/
///* MESSAGE ：RE障害ログ取得中止応答(CMT_CPRIF_REOBLOGSTPRES)				*/
///* 信号種別：0x5D20		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_REOBLOGSTPRES;
//
///****************************************************************************/
///* MESSAGE ：外部装置データ送信要求(CMT_CPRIF_EXTDATSNDREQ)					*/
///* 信号種別：0x9110		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				extdev_no;				/* 外部装置番号				*/
//	USHORT				snd_byte;				/* 送信バイト数				*/
//	UCHAR				snd_msg[2048];			/* 送信メッセージ			*/
//}CMT_CPRIF_EXTDATSNDREQ;
//
///****************************************************************************/
///* MESSAGE ：外部装置データ送信応答(CMT_CPRIF_EXTDATSNDRES)					*/
///* 信号種別：0x9120		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				extdev_no;				/* 外部装置番号				*/
//}CMT_CPRIF_EXTDATSNDRES;
//
///****************************************************************************/
///* MESSAGE ：外部装置データ受信要求(CMT_CPRIF_EXTDATRCVREQ)					*/
///* 信号種別：0x9130		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				extdev_no;				/* 外部装置番号				*/
//	USHORT				sndbyte_num;			/* 送信バイト数				*/
//	UCHAR				sndmsg[2048];			/* 送信メッセージ			*/
//}CMT_CPRIF_EXTDATRCVREQ;
//
///****************************************************************************/
///* MESSAGE ：外部装置データ受信応答(CMT_CPRIF_EXTDATRCVRES)					*/
///* 信号種別：0x9140		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				extdev_no;				/* 外部装置番号				*/
//}CMT_CPRIF_EXTDATRCVRES;
//
///****************************************************************************/
///* MESSAGE ：MTアドレス設定要求(CMT_CPRIF_MTADDSETREQ)						*/
///* 信号種別：0x9210		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				tgtmt;					/* 対象MT					*/
//	UCHAR				ipadd_1;				/* IPアドレス＃1			*/
//	UCHAR				ipadd_2;				/* IPアドレス＃2			*/
//	UCHAR				ipadd_3;				/* IPアドレス＃3			*/
//	UCHAR				ipadd_4;				/* IPアドレス＃4			*/
//	UCHAR				sbnetmsk_1;				/* サブネットマスク＃1		*/
//	UCHAR				sbnetmsk_2;				/* サブネットマスク＃2		*/
//	UCHAR				sbnetmsk_3;				/* サブネットマスク＃3		*/
//	UCHAR				sbnetmsk_4;				/* サブネットマスク＃4		*/
//	USHORT				cmdtrsport;				/* コマンド転送用ポート番号	*/
//	USHORT				filtrsport;				/* ファイル転送用ポート番号	*/
//	USHORT				portno;					/* ポート番号				*/
//}CMT_CPRIF_MTADDSETREQ;
//
///****************************************************************************/
///* MESSAGE ：MTアドレス設定応答(CMT_CPRIF_MTADDSETRES)						*/
///* 信号種別：0x9220		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_MTADDSETRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MT接続要求(CMT_CPRIF_FLDMTCNCREQ)							*/
///* 信号種別：0x9310		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//} CMT_CPRIF_FLDMTCNCREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MT接続応答(CMT_CPRIF_FLDMTCNCRES)							*/
///* 信号種別：0x9320		方向:TRIF->MDIF										*/
///*****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//
//} CMT_CPRIF_FLDMTCNCRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータコマンド転送送信要求(CMT_CPRIF_FLDCOMSNDREQ)		*/
///* 信号種別：0x9370		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				sndalldat_num;			/* 全送信データ数			*/
//	USHORT				snddatseq_no;			/* 送信データ順序番号		*/
//	USHORT				sndbyte_num;			/* 送信バイト数				*/
//	UCHAR				snddat[10240];			/* 送信データ				*/
//}CMT_CPRIF_FLDCOMSNDREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータコマンド転送送信応答(CMT_CPRIF_FLDCOMSNDRES)		*/
///* 信号種別：0x9380		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				sndalldat_num;			/* 全送信データ数			*/
//	USHORT				snddatseq_no;			/* 送信データ順序番号		*/
//}CMT_CPRIF_FLDCOMSNDRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータコマンド転送受信要求(CMT_CPRIF_FLDCOMRCVREQ)		*/
///* 信号種別：0x9390		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				rcvalldat_num;			/* 全受信データ数			*/
//	USHORT				rcvdatseq_no;			/* 受信データ順序番号		*/
//	USHORT				rcvbyte_num;			/* 受信バイト数				*/
//	UCHAR				rcvdat[10240];			/* 受信データ				*/
//}CMT_CPRIF_FLDCOMRCVREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータコマンド転送受信応答(CMT_CPRIF_FLDCOMRCVRES)		*/
///* 信号種別：0x93A0		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				rcvalldat_num;			/* 全受信データ数			*/
//	USHORT				rcvdatseq_no;			/* 受信データ順序番号		*/
//}CMT_CPRIF_FLDCOMRCVRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータファイル転送送信要求(CMT_CPRIF_FLDFILSNDREQ)		*/
///* 信号種別：0x93B0		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				protocol;				/* プロトコル				*/
//	USHORT				data_type;				/* FTP データ種別			*/
//	USHORT				sndbyte_num;			/* 送信バイト数				*/
//	UCHAR				snddat[4096];			/* 送信データ				*/
//}CMT_CPRIF_FLDFILSNDREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータファイル転送送信応答(CMT_CPRIF_FLDFILSNDRES)		*/
///* 信号種別：0x93C0		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_FLDFILSNDRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータファイル転送受信要求(CMT_CPRIF_FLDFILRCVREQ)		*/
///* 信号種別：0x93D0		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				protocol;				/* プロトコル				*/
//	USHORT				data_type;				/* FTP データ種別			*/
//	USHORT				rcvbyte_num;			/* 受信バイト数				*/
//	UCHAR				rcvdat[4096];			/* 受信データ				*/
//}CMT_CPRIF_FLDFILRCVREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MTデータファイル転送受信応答(CMT_CPRIF_FLDFILRCVRES)		*/
///* 信号種別：0x93E0		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_FLDFILRCVRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MT接続解放要求(CMT_CPRIF_FLMTCNCRELREQ)					*/
///* 信号種別：0x9330		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//} CMT_CPRIF_FLMTCNCRELREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MT接続解放応答(CMT_CPRIF_FLMTCNCRLSRES)					*/
///* 信号種別：0x9340		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//} CMT_CPRIF_FLMTCNCRELRES;
//
///****************************************************************************/
///* MESSAGE ：FLD-MT接続強制解放要求(CMT_CPRIF_FLMTCMPRELREQ)				*/
///* 信号種別：0x9350		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//} CMT_CPRIF_FLMTCMPRELREQ;
//
///****************************************************************************/
///* MESSAGE ：FLD-MT接続強制解放応答(CMT_CPRIF_FLMTCMPRELRES)				*/
///* 信号種別：0x9360		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//} CMT_CPRIF_FLMTCMPRELRES;
//
///****************************************************************************/
///* MESSAGE ：SV-MT接続要求(CMT_CPRIF_SVMTCNTREQ)							*/
///* 信号種別：0x9410		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_SVMTCNTREQ;
//
///****************************************************************************/
///* MESSAGE ：SV-MT接続応答(CMT_CPRIF_SVMTCNTRES)							*/
///* 信号種別：0x9420		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_SVMTCNTRES;
//
///****************************************************************************/
///* MESSAGE ：SV-MTデータ送信要求(CMT_CPRIF_SVMTDATSNDREQ)					*/
///* 信号種別：0x9470		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				snd_bytnum;				/* 送信バイト数				*/
//	UCHAR				snd_msg[10240];			/* 送信メッセージ			*/
//}CMT_CPRIF_SVMTDATSNDREQ;
//
///****************************************************************************/
///* MESSAGE ：SV-MTデータ送信応答(CMT_CPRIF_SVMTDATSNDRES)					*/
///* 信号種別：0x9480		方向：MDIF->TRIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_SVMTDATSNDRES;
//
///****************************************************************************/
///* MESSAGE ：SV-MTデータ受信要求(CMT_CPRIF_SVMTDATRCVREQ)					*/
///* 信号種別：0x9490		方向：MDIF->TRIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				rcv_bytnum;				/* 受信バイト数				*/
//	UCHAR				rcv_msg[10240];			/* 受信メッセージ			*/
//}CMT_CPRIF_SVMTDATRCVREQ;
//
///****************************************************************************/
///* MESSAGE ：SV-MTデータ受信応答(CMT_CPRIF_SVMTDATRCVRES)					*/
///* 信号種別：0x94A0		方向：TRIF->MDIF									*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_SVMTDATRCVRES;
//
///****************************************************************************/
///* MESSAGE ：SV-MT接続解放要求(CMT_CPRIF_SVMTCNTRELREQ)						*/
///* 信号種別：0x9430		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_SVMTCNTRELREQ;
//
///****************************************************************************/
///* MESSAGE ：SV-MT接続解放応答(CMT_CPRIF_SVMTCNTRELRES)						*/
///* 信号種別：0x9440		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_SVMTCNTRELRES;
//
///****************************************************************************/
///* MESSAGE ：SV-MT接続強制解放要求(CMT_CPRIF_SVMTFORRELREQ)					*/
///* 信号種別：0x9450		方向:TRIF->MDIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//}CMT_CPRIF_SVMTFORRELREQ;
//
///****************************************************************************/
///* MESSAGE ：SV-MT接続強制解放応答(CMT_CPRIF_SVMTFORRELRES)					*/
///* 信号種別：0x9460		方向:MDIF->TRIF										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_CPRIF_SVMTFORRELRES;
//
///****************************************************************************/
///* MESSAGE ：メーカMTデータ送信(CMT_CPRIF_MKMTDATSND)						*/
///* 信号種別：0x9500		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTDATSND;
//
///****************************************************************************/
///* MESSAGE ：メーカMT接続要求(CMT_CPRIF_MKMTCNTREQ)							*/
///* 信号種別：0x9510		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTCNTREQ;
//
///****************************************************************************/
///* MESSAGE ：メーカMT接続応答(CMT_CPRIF_MKMTCNTRES) 						*/
///* 信号種別：0x9520		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTCNTRES;
//
///****************************************************************************/
///* MESSAGE ：メーカMT接続解放要求(CMT_CPRIF_MKMTCNTRELREQ)					*/
///* 信号種別：0x9530		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTCNTRELREQ;
//
///****************************************************************************/
///* MESSAGE ：メーカMT接続解放応答(CMT_CPRIF_MKMTCNTRELRES)					*/
///* 信号種別：0x9540		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTCNTRELRES;
//
///****************************************************************************/
///* MESSAGE ：メーカMT接続強制解放要求(CMT_CPRIF_MKMTFORRELREQ) 				*/
///* 信号種別：0x9550		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTFORRELREQ;
//
///****************************************************************************/
///* MESSAGE ：メーカMT接続強制解放応答(CMT_CPRIF_MKMTFORRELRES) 				*/
///* 信号種別：0x9560		方向:TRIF->MDIF、MDIF->TRIF							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				dummy;					/* dummy領域				*/
//}CMT_CPRIF_MKMTFORRELRES;
//
//
///****************************************************************************/
///* MESSAGE ：ダミーL3メッセージ								 				*/
///* 信号種別：0xFFF1/0xFFE1		方向:MUX->TRIF								*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				signal_kind;			/* 信号種別					*/
//	USHORT				log_size;				/* 取得ログサイズ			*/
//	UINT				reg_log[12];			/* 取得ログ					*/
//}CMT_CPRIF_MUXDUMMYFRM;
//
//
///****************************************************************************/
///* MESSAGE :CPRI信号受信通知(CMT_TSKIF_CPRIRCVNTC)							*/
///* 起動要因:0x03040005														*/
///****************************************************************************/
///* CPRI信号受信情報部 */
//typedef struct
//{
//	USHORT						link_num;		/* CPRI番号					*/
//	UCHAR						dummy1;			/* ダミー領域	1			*/
//	UCHAR						dummy2;			/* ダミー領域	2			*/
//	UINT					i_link;				/* I queue pointer			*/
//	UINT					length;				/* buffer length			*/
//	UINT					disp;				/* buffer offset sizes		*/
//	UCHAR					Padding1;			/* Padding1					*/
//	UCHAR					Padding2;			/* Padding2					*/
//	UCHAR					Address;			/* HDLC frame address area	*/
//	UCHAR					Control;			/* HDLC frame control area	*/
//}CMT_CPRISIGRCV_INF;
//
///* CPRI信号受信通知(信号種別取得用)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	USHORT					signal_kind;		/* 信号種別					*/
//	USHORT					dummy;				/* dummy領域				*/
//} CMT_TSKIF_CPRIRCV_SIGNALGET;
//
///* CPRI信号受信通知(ヘルスチェック要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_HCREQ			cpridat_hcreq;		/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_HCREQ;
//
///* CPRI信号受信通知(ヘルスチェック応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_HCRES			cpridat_hcres;		/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_HCRES;
//
///* CPRI信号受信通知(REリセット要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_RERSTREQ		cpridat_rerstreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_RERSTREQ;
//
///* CPRI信号受信通知(RE時刻設定要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RETIMSETREQ	cpridat_retimsetreq;
//} CMT_TSKIF_CPRIRCV_RETIMSETREQ;
//
///* CPRI信号受信通知(RE共通部リンク確立要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_LINKSTSREQ	cpridat_linkstsreq; /* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_LINKSTSREQ;
//
///* CPRI信号受信通知(RE共通部リンク確立応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_LINKSTSRES	cpridat_linkstsres; /* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_LINKSTSRES;
//
///* CPRI信号受信通知(ファームウェアファイル報告要求)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_FIRMFILEREQ	cpridat_firmfilereq;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_FIRMFILEREQ;
//
///* CPRI信号受信通知(ファームウェアファイル報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_FIRMFILERES	cpridat_firmfileres;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_FIRMFILERES;
//
///* CPRI信号受信通知(ファイル情報報告通知)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_FILEINFREQ	cpridat_fileinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_FILEINFREQ;
//
///* CPRI信号受信通知(ファイル情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_FILEINFRES	cpridat_fileinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_FILEINFRES;
//
///* CPRI信号受信通知(ファイルデータ送信)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_FILEDATSND	cpridat_filedatsnd;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_FILEDATSND;
//
///* CPRI信号受信通知(ファイルデータ送信完了報告通知)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FILSNDFINNTC	cpridat_filsndfinntc;
//} CMT_TSKIF_CPRIRCV_FILSNDFINNTC;
//
///* CPRI信号受信通知(ファイルデータ送信完了報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FILSNDFINRES	cpridat_filsndfinres;
//} CMT_TSKIF_CPRIRCV_FILSNDFINRES;
//
///* CPRI信号受信通知(運用中ファームウェアファイル報告要求)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_UFIRMFILEREQ	cpridat_ufirmfilereq;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_UFIRMFILEREQ;
//
///* CPRI信号受信通知(運用中ファームウェアファイル報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_UFIRMFILERES	cpridat_ufirmfileres;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_UFIRMFILERES;
//
///* CPRI信号受信通知(運用中ファイル情報報告通知)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_UFILEINFREQ	cpridat_ufileinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_UFILEINFREQ;
//
///* CPRI信号受信通知(運用中ファイル情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_UFILEINFRES	cpridat_ufileinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_UFILEINFRES;
//
///* CPRI信号受信通知(運用中ファイルデータ送信)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_UFILEDATSND	cpridat_ufiledatsnd;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_UFILEDATSND;
//
///* CPRI信号受信通知(運用中ファイルデータ送信完了報告通知)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_UFILSNDFINNTC	cpridat_ufilsndfinntc;
//} CMT_TSKIF_CPRIRCV_UFILSNDFINNTC;
//
///* CPRI信号受信通知(運用中ファイルデータ送信完了報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_UFILSNDFINRES	cpridat_ufilsndfinres;
//} CMT_TSKIF_CPRIRCV_UFILSNDFINRES;
//
///* CPRI信号受信通知(TRX設定要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_TRXSETREQ		cpridat_trxsetreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_TRXSETREQ;
//
///* CPRI信号受信通知(TRX設定応答)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_TRXSETRES		cpridat_trxsetres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_TRXSETRES;
//
///* CPRI信号受信通知(TRX解放要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_TRXRELREQ		cpridat_trxrelreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_TRXRELREQ;
//
///* CPRI信号受信通知(TRX解放応答)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_TRXRELRES		cpridat_trxrelres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_TRXRELRES;
//
///* CPRI信号受信通知(Toffset値報告要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_TOFFSETINFREQ	cpridat_toffsetinfreq;
//} CMT_TSKIF_CPRIRCV_TOFFSETINFREQ;
//
///* CPRI信号受信通知(Toffset値報告応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_TOFFSETINFRES	cpridat_toffsetinfres;
//} CMT_TSKIF_CPRIRCV_TOFFSETINFRES;
//
///* CPRI信号受信通知(無変調キャリア出力ON/OFF指定要求)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_CARONOFFREQ	cpridat_caronoffreq;
//} CMT_TSKIF_CPRIRCV_CARONOFFREQ;
//
///* CPRI信号受信通知(無変調キャリア出力ON/OFF指定応答)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_CARONOFFRES	cpridat_caronoffres;
//} CMT_TSKIF_CPRIRCV_CARONOFFRES;
//
///* CPRI信号受信通知(RE装置構成情報報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_EQPSTRREQ		cpridat_eqpstrreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_EQPSTRREQ;
//
///* CPRI信号受信通知(RE装置構成情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_EQPSTRRES		cpridat_eqpstrres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_EQPSTRRES;
//
///* CPRI信号受信通知(REスロット情報報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_SLOTINFREQ	cpridat_slotinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_SLOTINFREQ;
//
///* CPRI信号受信通知(REスロット情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_SLOTINFRES	cpridat_slotinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_SLOTINFRES;
//
///* CPRI信号受信通知(RE状態報告要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_RESTSREQ		cpridat_restsreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_RESTSREQ;
//
///* CPRI信号受信通知(RE状態報告応答)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_RESTSRES		cpridat_restsres;	/* CPRI信号データ部			*/
//												/* .S @001					*/
///* { CMT_TSKIF_CPRIRCV_RESTSREQ; */
//} CMT_TSKIF_CPRIRCV_RESTSRES;					/* .E @001					*/
//
///* CPRI信号受信通知(REカード情報報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_CARDINFREQ	cpridat_cardinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_CARDINFREQ;
//
///* CPRI信号受信通知(REカード情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_CARDINFRES	cpridat_cardinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_CARDINFRES;
//
///* CPRI信号受信通知(REバージョン報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_REVERREQ		cpridat_reverreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_REVERREQ;
//
///* CPRI信号受信通知(REバージョン報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_REVERRES		cpridat_reverres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_REVERRES;
//
///* CPRI信号受信通知(REカード制御要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECARDCNTREQ	cpridat_recardcntreq;
//} CMT_TSKIF_CPRIRCV_RECARDCNTREQ;
//
///* CPRI信号受信通知(REカード制御応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECARDCNTRES	cpridat_recardcntres;
//} CMT_TSKIF_CPRIRCV_RECARDCNTRES;
//
///* CPRI信号受信通知(REカード状態報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_CARDSTSREQ	cpridat_cardstsreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_CARDSTSREQ;
//
///* CPRI信号受信通知(REカード状態報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_CARDSTSRES	cpridat_cardstsres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_CARDSTSRES;
//
///* CPRI信号受信通知(REキャリア状態報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_CARLSTSREQ	cpridat_carstsreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_CARLSTSREQ;
//
///* CPRI信号受信通知(REキャリア状態報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_CARLSTSRES	cpridat_carstsres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_CARLSTSRES;
//
///* CPRI信号受信通知(RE PORT制御要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTCNTREQ	cpridat_reportcntreq;
//} CMT_TSKIF_CPRIRCV_REPORTCNTREQ;
//
///* CPRI信号受信通知(RE PORT制御応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTCNTRES	cpridat_reportcntres;
//} CMT_TSKIF_CPRIRCV_REPORTCNTRES;
//
///* CPRI信号受信通知(RE PORT状態報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTSTSREQ	cpridat_reportstsreq;
//} CMT_TSKIF_CPRIRCV_REPORTSTSREQ;
//
///* CPRI信号受信通知(RE PORT状態報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTSTSRES	cpridat_reportstsres;
//} CMT_TSKIF_CPRIRCV_REPORTSTSRES;
//
///* CPRI信号受信通知(REカード診断要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECRDDIAREQ	cpridat_recrddiareq;
//} CMT_TSKIF_CPRIRCV_RECRDDIAREQ;
//
///* CPRI信号受信通知(REカード診断応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECRDDIARES	cpridat_recrddiares;
//} CMT_TSKIF_CPRIRCV_RECRDDIARES;
//
///* CPRI信号受信通知(RE障害ログ取得要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBSLOGREQ	cpridat_reobslogreq;
//} CMT_TSKIF_CPRIRCV_REOBSLOGREQ;
//
///* CPRI信号受信通知(RE障害ログ取得応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBSLOGRES	cpridat_reobslogres;
//} CMT_TSKIF_CPRIRCV_REOBSLOGRES;
//
///* CPRI信号受信通知(RE障害ログ取得中止要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBLOGSTPREQ	cpridat_reoblogstpreq;
//} CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ;
//
///* CPRI信号受信通知(RE障害ログ取得中止応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBLOGSTPRES	cpridat_reoblogstpres;
//} CMT_TSKIF_CPRIRCV_REOBLOGSTPRES;
//
///* CPRI信号受信通知(外部装置データ送信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATSNDREQ	cpridat_extdatsndreq;
//} CMT_TSKIF_CPRIRCV_EXTDATSNDREQ;
//
///* CPRI信号受信通知(外部装置データ送信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATSNDRES	cpridat_extdatsndres;
//} CMT_TSKIF_CPRIRCV_EXTDATSNDRES;
//
///* CPRI信号受信通知(外部装置データ受信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATRCVREQ	cpridat_extdatrcvreq;
//} CMT_TSKIF_CPRIRCV_EXTDATRCVREQ;
//
///* CPRI信号受信通知(外部装置データ受信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATRCVRES	cpridat_extdatrcvres;
//} CMT_TSKIF_CPRIRCV_EXTDATRCVRES;
//
///* CPRI信号受信通知(MTアドレス設定要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MTADDSETREQ	cpridat_mtaddsetreq;
//} CMT_TSKIF_CPRIRCV_MTADDSETREQ;
//
///* CPRI信号受信通知(MTアドレス設定応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MTADDSETRES	cpridat_mtaddsetres;
//} CMT_TSKIF_CPRIRCV_MTADDSETRES;
//
///* CPRI信号受信通知(FLD-MT接続要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDMTCNCREQ	cpridat_fldmtcncreq;
//} CMT_TSKIF_CPRIRCV_FLDMTCNCREQ;
//
///* CPRI信号受信通知(FLD-MT接続応答)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDMTCNCRES	cpridat_fldmtcncres;
//} CMT_TSKIF_CPRIRCV_FLDMTCNCRES;
//
///* CPRI信号受信通知(FLD-MTデータコマンド転送送信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMSNDREQ	cpridat_fldcomsndreq;
//} CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ;
//
///* CPRI信号受信通知(FLD-MTデータコマンド転送送信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMSNDRES	cpridat_fldcomsndres;
//} CMT_TSKIF_CPRIRCV_FLDCOMSNDRES;
//
///* CPRI信号受信通知(FLD-MTデータコマンド転送受信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMRCVREQ	cpridat_fldcomrcvreq;
//} CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ;
//
///* CPRI信号受信通知(FLD-MTデータコマンド転送受信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMRCVRES	cpridat_fldcomrcvres;
//} CMT_TSKIF_CPRIRCV_FLDCOMRCVRES;
//
///* CPRI信号受信通知(FLD-MTデータファイル転送送信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILSNDREQ	cpridat_fldfilsndreq;
//} CMT_TSKIF_CPRIRCV_FLDFILSNDREQ;
//
///* CPRI信号受信通知(FLD-MTデータファイル転送送信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILSNDRES	cpridat_fldfilsndres;
//} CMT_TSKIF_CPRIRCV_FLDFILSNDRES;
//
///* CPRI信号受信通知(FLD-MTデータファイル転送受信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILRCVREQ	cpridat_fldfilrcvreq;
//} CMT_TSKIF_CPRIRCV_FLDFILRCVREQ;
//
///* CPRI信号受信通知(FLD-MTデータファイル転送受信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILRCVRES	cpridat_fldfilrcvres;
//} CMT_TSKIF_CPRIRCV_FLDFILRCVRES;
//
///* CPRI信号受信通知(FLD-MT接続解放要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCNCRELREQ	cpridat_flmtcncrelreq;
//} CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ;
//
///* CPRI信号受信通知(FLD-MT接続解放応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCNCRELRES	cpridat_flmtcncrelres;
//} CMT_TSKIF_CPRIRCV_FLMTCNCRELRES;
//
///* CPRI信号受信通知(FLD-MT接続強制解放要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCMPRELREQ	cpridat_flmtcmprelreq;
//} CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ;
//
///* CPRI信号受信通知(FLD-MT接続強制解放応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCMPRELRES	cpridat_flmtcmprelres;
//} CMT_TSKIF_CPRIRCV_FLMTCMPRELRES;
//
///* CPRI信号受信通知(SV-MT接続要求)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_SVMTCNTREQ	cpridat_svmtcntreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_SVMTCNTREQ;
//
///* CPRI信号受信通知(SV-MT接続応答)											*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_SVMTCNTRES	cpridat_svmtcntres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_SVMTCNTRES;
//
///* CPRI信号受信通知(SV-MTデータ送信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATSNDREQ	cpridat_svmtdatsndreq;
//} CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ;
//
///* CPRI信号受信通知(SV-MTデータ送信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATSNDRES	cpridat_svmtdatsndres;
//} CMT_TSKIF_CPRIRCV_SVMTDATSNDRES;
//
///* CPRI信号受信通知(SV-MTデータ受信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATRCVREQ	cpridat_svmtdatrcvreq;
//} CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ;
//
///* CPRI信号受信通知(SV-MTデータ受信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATRCVRES	cpridat_svmtdatrcvres;
//} CMT_TSKIF_CPRIRCV_SVMTDATRCVRES;
//
///* CPRI信号受信通知(SV-MT接続解放要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTCNTRELREQ	cpridat_svmtcntrelreq;
//} CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ;
//
///* CPRI信号受信通知(SV-MT接続解放応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTCNTRELRES	cpridat_svmtcntrelres;
//} CMT_TSKIF_CPRIRCV_SVMTCNTRELRES;
//
///* CPRI信号受信通知(SV-MT接続強制解放要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTFORRELREQ	cpridat_svmtforrelreq;
//} CMT_TSKIF_CPRIRCV_SVMTFORRELREQ;
//
///* CPRI信号受信通知(SV-MT接続強制解放応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTFORRELRES	cpridat_svmtforrelres;
//} CMT_TSKIF_CPRIRCV_SVMTFORRELRES;
//
///* CPRI信号受信通知(メーカMTデータ送信)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_MKMTDATSND	cpridat_mkmtdatsnd;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_MKMTDATSND;
//
///* CPRI信号受信通知(メーカMT接続要求)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_MKMTCNTREQ	cpridat_mkmtcntreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_MKMTCNTREQ;
//
///* CPRI信号受信通知(メーカMT接続応答)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//	CMT_CPRIF_MKMTCNTRES	cpridat_mkmtcntres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRIRCV_MKMTCNTRES;
//
///* CPRI信号受信通知(メーカMT接続解放要求)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTCNTRELREQ	cpridat_mkmtcntrelreq;
//} CMT_TSKIF_CPRIRCV_MKMTCNTRELREQ;
//
///* CPRI信号受信通知(メーカMT接続解放応答)									*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTCNTRELRES	cpridat_mkmtcntrelres;
//} CMT_TSKIF_CPRIRCV_MKMTCNTRELRES;
//
///* CPRI信号受信通知(メーカMT接続強制解放要求)								*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTFORRELREQ	cpridat_mkmtforrelreq;
//} CMT_TSKIF_CPRIRCV_MKMTFORRELREQ;
//
///* CPRI信号受信通知(メーカMT接続強制解放応答)								*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTFORRELRES	cpridat_mkmtforrelres;
//} CMT_TSKIF_CPRIRCV_MKMTFORRELRES;
//
//
///* CPRI信号受信通知(ダミーL3メッセージ)										*/
//typedef struct
//{
//	CMT_CPRISIGRCV_INF		cprircv_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MUXDUMMYFRM	cpridat_muxdummyfrm;
//} CMT_TSKIF_CPRIRCV_MUXDUMMYFRM;
//
//
///****************************************************************************/
///* MESSAGE :CPRI信号送信指示(CMT_TSKIF_CPRISNDNTC)							*/
///* 起動要因:0x03050004														*/
///****************************************************************************/
///* CPRI信号送信情報部 */
//typedef struct
//{
//	USHORT						link_num;		/* CPRI番号					*/
//	UCHAR						dummy1;			/* ダミー領域	1			*/
//	UCHAR						dummy2;			/* ダミー領域	2			*/
//	UINT					i_link;				/* I queue pointer			*/
//	UINT					length;				/* buffer length			*/
//	UINT					disp;				/* buffer offset sizes		*/
//	UCHAR					Padding1;			/* Padding1					*/
//	UCHAR					Padding2;			/* Padding2					*/
//	UCHAR					Address;			/* HDLC frame address area	*/
//	UCHAR					Control;			/* HDLC frame control area	*/
//}CMT_CPRISIGSND_INF;
//
///* CPRI信号送信指示(信号種別取得用)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	USHORT					signal_kind;		/* 信号種別					*/
//	USHORT					dummy;				/* dummy領域				*/
//} CMT_TSKIF_CPRISND_SIGNALGET;
//
///* CPRI信号送信指示(ヘルスチェック要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_HCREQ			cpridat_hcreq;		/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_HCREQ;
//
///* CPRI信号送信指示(ヘルスチェック応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_HCRES			cpridat_hcres;		/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_HCRES;
//
///* CPRI信号送信指示(REリセット要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_RERSTREQ		cpridat_rerstreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_RERSTREQ;
//
///* CPRI信号送信指示(RE時刻設定要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RETIMSETREQ	cpridat_retimsetreq;
//} CMT_TSKIF_CPRISND_RETIMSETREQ;
//
///* CPRI信号送信指示(RE共通部リンク確立要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_LINKSTSREQ	cpridat_linkstsreq; /* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_LINKSTSREQ;
//
///* CPRI信号送信指示(RE共通部リンク確立応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_LINKSTSRES	cpridat_linkstsres; /* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_LINKSTSRES;
//
///* CPRI信号送信指示(ファームウェアファイル報告要求)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_FIRMFILEREQ	cpridat_firmfilereq;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_FIRMFILEREQ;
//
///* CPRI信号送信指示(ファームウェアファイル報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_FIRMFILERES	cpridat_firmfileres;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_FIRMFILERES;
//
///* CPRI信号送信指示(ファイル情報報告通知)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_FILEINFREQ	cpridat_fileinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_FILEINFREQ;
//
///* CPRI信号送信指示(ファイル情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_FILEINFRES	cpridat_fileinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_FILEINFRES;
//
///* CPRI信号送信指示(ファイルデータ送信)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_FILEDATSND	cpridat_filedatsnd;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_FILEDATSND;
//
///* CPRI信号送信指示(ファイルデータ送信完了報告通知)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FILSNDFINNTC	cpridat_filsndfinntc;
//} CMT_TSKIF_CPRISND_FILSNDFINNTC;
//
///* CPRI信号送信指示(ファイルデータ送信完了報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FILSNDFINRES	cpridat_filsndfinres;
//} CMT_TSKIF_CPRISND_FILSNDFINRES;
//
///* CPRI信号送信指示(運用中ファームウェアファイル報告要求)	*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFIRMFILEREQ	cpridat_ufirmfilereq;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFIRMFILEREQ;
//
///* CPRI信号送信指示(運用中ファームウェアファイル報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFIRMFILERES	cpridat_ufirmfileres;/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFIRMFILERES;
//
///* CPRI信号送信指示(運用中ファイル情報報告通知)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFILEINFREQ	cpridat_ufileinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFILEINFREQ;
//
///* CPRI信号送信指示(運用中ファイル情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFILEINFRES	cpridat_ufileinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFILEINFRES;
//
///* CPRI信号送信指示(運用中ファイルデータ送信)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFILEDATSND	cpridat_ufiledatsnd;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFILEDATSND;
//
///* CPRI信号送信指示(運用中ファイルデータ送信完了報告通知)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFILSNDFINNTC	cpridat_ufilsndfinntc;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFILSNDFINNTC;
//
///* CPRI信号送信指示(運用中ファイルデータ送信完了報告応答)							*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_UFILSNDFINRES	cpridat_ufilsndfinres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_UFILSNDFINRES;
//
///* CPRI信号送信指示(TRX設定要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_TRXSETREQ		cpridat_trxsetreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_TRXSETREQ;
//
///* CPRI信号送信指示(TRX設定応答)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_TRXSETRES		cpridat_trxsetres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_TRXSETRES;
//
///* CPRI信号送信指示(TRX解放要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_TRXRELREQ		cpridat_trxrelreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_TRXRELREQ;
//
///* CPRI信号送信指示(TRX解放応答)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_TRXRELRES		cpridat_trxrelres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_TRXRELRES;
//
///* CPRI信号送信指示(Toffset値報告要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_TOFFSETINFREQ	cpridat_toffsetinfreq;
//} CMT_TSKIF_CPRISND_TOFFSETINFREQ;
//
///* CPRI信号送信指示(Toffset値報告応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_TOFFSETINFRES	cpridat_toffsetinfres;
//} CMT_TSKIF_CPRISND_TOFFSETINFRES;
//
///* CPRI信号送信指示(無変調キャリア出力ON/OFF指定要求)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_CARONOFFREQ	cpridat_caronoffreq;
//} CMT_TSKIF_CPRISND_CARONOFFREQ;
//
///* CPRI信号送信指示(無変調キャリア出力ON/OFF指定応答)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_CARONOFFRES	cpridat_caronoffres;
//} CMT_TSKIF_CPRISND_CARONOFFRES;
//
///* CPRI信号送信指示(RE装置構成情報報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_EQPSTRREQ		cpridat_eqpstrreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_EQPSTRREQ;
//
///* CPRI信号送信指示(RE装置構成情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_EQPSTRRES		cpridat_eqpstrres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_EQPSTRRES;
//
///* CPRI信号送信指示(REスロット情報報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_SLOTINFREQ	cpridat_slotinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_SLOTINFREQ;
//
///* CPRI信号送信指示(REスロット情報報告応答)									*/
///* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			*/
///*	 に本フレームの使用はできない。											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_SLOTINFRES	cpridat_slotinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_SLOTINFRES;
//
///* CPRI信号送信指示(RE状態報告要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_RESTSREQ		cpridat_restsreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_RESTSREQ;
//
///* CPRI信号送信指示(RE状態報告応答)											*/
///* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			*/
///*	 に本フレームの使用はできない。											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_RESTSRES		cpridat_restsres;	/* CPRI信号データ部			*/
//												/* .S @001					*/
///* } CMT_TSKIF_CPRISND_RESTSREQ; */
//} CMT_TSKIF_CPRISND_RESTSRES;					/* .E @001					*/
//
///* CPRI信号送信指示(REカード情報報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_CARDINFREQ	cpridat_cardinfreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_CARDINFREQ;
//
///* CPRI信号送信指示(REカード情報報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_CARDINFRES	cpridat_cardinfres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_CARDINFRES;
//
///* CPRI信号送信指示(REバージョン報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_REVERREQ		cpridat_reverreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_REVERREQ;
//
///* CPRI信号送信指示(REバージョン報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_REVERRES		cpridat_reverres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_REVERRES;
//
///* CPRI信号送信指示(REカード制御要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECARDCNTREQ	cpridat_recardcntreq;
//} CMT_TSKIF_CPRISND_RECARDCNTREQ;
//
//
///* CPRI信号送信指示(REカード制御応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECARDCNTRES	cpridat_recardcntres;
//} CMT_TSKIF_CPRISND_RECARDCNTRES;
//
//
///* CPRI信号送信指示(REカード状態報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_CARDSTSREQ	cpridat_cardstsreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_CARDSTSREQ;
//
//
///* CPRI信号送信指示(REカード状態報告応答)									*/
///* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			*/
///*	 に本フレームの使用はできない。											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_CARDSTSRES	cpridat_cardstsres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_CARDSTSRES;
//
//
///* CPRI信号送信指示(REキャリア状態報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_CARLSTSREQ	cpridat_carstsreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_CARLSTSREQ;
//
///* CPRI信号送信指示(REキャリア状態報告応答)									*/
///* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			*/
///*	 に本フレームの使用はできない。											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_CARLSTSRES	cpridat_carstsres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_CARLSTSRES;
//
///* CPRI信号送信指示(RE PORT制御要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTCNTREQ	cpridat_reportcntreq;
//} CMT_TSKIF_CPRISND_REPORTCNTREQ;
//
///* CPRI信号送信指示(RE PORT制御応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTCNTRES	cpridat_reportcntres;
//} CMT_TSKIF_CPRISND_REPORTCNTRES;
//
///* CPRI信号送信指示(RE PORT状態報告要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTSTSREQ	cpridat_reportstsreq;
//} CMT_TSKIF_CPRISND_REPORTSTSREQ;
//
///* CPRI信号送信指示(RE PORT状態報告応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REPORTSTSRES	cpridat_reportstsres;
//} CMT_TSKIF_CPRISND_REPORTSTSRES;
//
///* CPRI信号送信指示(REカード診断要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECRDDIAREQ	cpridat_recrddiareq;
//} CMT_TSKIF_CPRISND_RECRDDIAREQ;
//
///* CPRI信号送信指示(REカード診断応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_RECRDDIARES	cpridat_recrddiares;
//} CMT_TSKIF_CPRISND_RECRDDIARES;
//
///* CPRI信号送信指示(RE障害ログ取得要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBSLOGREQ	cpridat_reobslogreq;
//} CMT_TSKIF_CPRISND_REOBSLOGREQ;
//
///* CPRI信号送信指示(RE障害ログ取得応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBSLOGRES	cpridat_reobslogres;
//} CMT_TSKIF_CPRISND_REOBSLOGRES;
//
//
///* CPRI信号送信指示(RE障害ログ取得中止要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号受信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBLOGSTPREQ	cpridat_reoblogstpreq;
//} CMT_TSKIF_CPRISND_REOBLOGSTPREQ;
//
//
///* CPRI信号送信指示(RE障害ログ取得中止応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_REOBLOGSTPRES	cpridat_reoblogstpres;
//} CMT_TSKIF_CPRISND_REOBLOGSTPRES;
//
///* CPRI信号送信指示(外部装置データ送信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATSNDREQ	cpridat_extdatsndreq;
//} CMT_TSKIF_CPRISND_EXTDATSNDREQ;
//
///* CPRI信号送信指示(外部装置データ送信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATSNDRES	cpridat_extdatsndres;
//} CMT_TSKIF_CPRISND_EXTDATSNDRES;
//
///* CPRI信号送信指示(外部装置データ受信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATRCVREQ	cpridat_extdatrcvreq;
//} CMT_TSKIF_CPRISND_EXTDATRCVREQ;
//
///* CPRI信号送信指示(外部装置データ受信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_EXTDATRCVRES	cpridat_extdatrcvres;
//} CMT_TSKIF_CPRISND_EXTDATRCVRES;
//
///* CPRI信号送信指示(MTアドレス設定要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MTADDSETREQ	cpridat_mtaddsetreq;
//} CMT_TSKIF_CPRISND_MTADDSETREQ;
//
///* CPRI信号送信指示(FLD-MT接続要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDMTCNCREQ	cpridat_fldmtcncreq;
//} CMT_TSKIF_CPRISND_FLDMTCNCREQ;
//
///* CPRI信号送信指示(FLD-MT接続応答)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDMTCNCRES	cpridat_fldmtcncres;
//} CMT_TSKIF_CPRISND_FLDMTCNCRES;
//
///* CPRI信号送信指示(FLD-MTデータコマンド転送送信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMSNDREQ	cpridat_fldcomsndreq;
//} CMT_TSKIF_CPRISND_FLDCOMSNDREQ;
//
///* CPRI信号送信指示(FLD-MTデータコマンド転送送信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMSNDRES	cpridat_fldcomsndres;
//} CMT_TSKIF_CPRISND_FLDCOMSNDRES;
//
///* CPRI信号送信指示(FLD-MTデータコマンド転送受信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMRCVREQ	cpridat_fldcomrcvreq;
//} CMT_TSKIF_CPRISND_FLDCOMRCVREQ;
//
///* CPRI信号送信指示(FLD-MTデータコマンド転送受信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDCOMRCVRES	cpridat_fldcomrcvres;
//} CMT_TSKIF_CPRISND_FLDCOMRCVRES;
//
///* CPRI信号送信指示(FLD-MTデータファイル転送送信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILSNDREQ	cpridat_fldfilsndreq;
//} CMT_TSKIF_CPRISND_FLDFILSNDREQ;
//
///* CPRI信号送信指示(FLD-MTデータファイル転送送信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILSNDRES	cpridat_fldfilsndres;
//} CMT_TSKIF_CPRISND_FLDFILSNDRES;
//
///* CPRI信号送信指示(FLD-MTデータファイル転送受信要求)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILRCVREQ	cpridat_fldfilrcvreq;
//} CMT_TSKIF_CPRISND_FLDFILRCVREQ;
//
///* CPRI信号送信指示(FLD-MTデータファイル転送受信応答)						*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLDFILRCVRES	cpridat_fldfilrcvres;
//} CMT_TSKIF_CPRISND_FLDFILRCVRES;
//
///* CPRI信号送信指示(FLD-MT接続解放要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCNCRELREQ	cpridat_flmtcncrelreq;
//} CMT_TSKIF_CPRISND_FLMTCNCRELREQ;
//
///* CPRI信号送信指示(FLD-MT接続解放応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCNCRELRES	cpridat_flmtcncrelres;
//} CMT_TSKIF_CPRISND_FLMTCNCRELRES;
//
///* CPRI信号送信指示(FLD-MT接続強制解放要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCMPRELREQ	cpridat_flmtcmprelreq;
//} CMT_TSKIF_CPRISND_FLMTCMPRELREQ;
//
///* CPRI信号送信指示(FLD-MT接続強制解放応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_FLMTCMPRELRES	cpridat_flmtcmprelres;
//} CMT_TSKIF_CPRISND_FLMTCMPRELRES;
//
///* CPRI信号送信指示(SV-MT接続要求)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_SVMTCNTREQ	cpridat_svmtcntreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_SVMTCNTREQ;
//
///* CPRI信号送信指示(SV-MT接続応答)											*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_SVMTCNTRES	cpridat_svmtcntres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_SVMTCNTRES;
//
///* CPRI信号送信指示(SV-MTデータ送信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATSNDREQ	cpridat_svmtdatsndreq;
//} CMT_TSKIF_CPRISND_SVMTDATSNDREQ;
//
///* CPRI信号送信指示(SV-MTデータ送信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATSNDRES	cpridat_svmtdatsndres;
//} CMT_TSKIF_CPRISND_SVMTDATSNDRES;
//
///* CPRI信号送信指示(SV-MTデータ受信要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATRCVREQ	cpridat_svmtdatrcvreq;
//} CMT_TSKIF_CPRISND_SVMTDATRCVREQ;
//
///* CPRI信号送信指示(SV-MTデータ受信応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTDATRCVRES	cpridat_svmtdatrcvres;
//} CMT_TSKIF_CPRISND_SVMTDATRCVRES;
//
///* CPRI信号送信指示(SV-MT接続解放要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTCNTRELREQ	cpridat_svmtcntrelreq;
//} CMT_TSKIF_CPRISND_SVMTCNTRELREQ;
//
///* CPRI信号送信指示(SV-MT接続解放応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTCNTRELRES	cpridat_svmtcntrelres;
//} CMT_TSKIF_CPRISND_SVMTCNTRELRES;
//
///* CPRI信号送信指示(SV-MT接続強制解放要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTFORRELREQ	cpridat_svmtforrelreq;
//} CMT_TSKIF_CPRISND_SVMTFORRELREQ;
//
///* CPRI信号送信指示(SV-MT接続強制解放応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_SVMTFORRELRES	cpridat_svmtforrelres;
//} CMT_TSKIF_CPRISND_SVMTFORRELRES;
//
///* CPRI信号送信指示(メーカMTデータ送信)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_MKMTDATSND	cpridat_mkmtdatsnd;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_MKMTDATSND;
//
///* CPRI信号送信指示(メーカMT接続要求)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_MKMTCNTREQ	cpridat_mkmtcntreq;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_MKMTCNTREQ;
//
///* CPRI信号送信指示(メーカMT接続応答)										*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//	CMT_CPRIF_MKMTCNTRES	cpridat_mkmtcntres;	/* CPRI信号データ部			*/
//} CMT_TSKIF_CPRISND_MKMTCNTRES;
//
///* CPRI信号送信指示(メーカMT接続解放要求)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTCNTRELREQ	cpridat_mkmtcntrelreq;
//} CMT_TSKIF_CPRISND_MKMTCNTRELREQ;
//
///* CPRI信号送信指示(メーカMT接続解放応答)									*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTCNTRELRES	cpridat_mkmtcntrelres;
//} CMT_TSKIF_CPRISND_MKMTCNTRELRES;
//
///* CPRI信号送信指示(メーカMT接続強制解放要求)								*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTFORRELREQ	cpridat_mkmtforrelreq;
//} CMT_TSKIF_CPRISND_MKMTFORRELREQ;
//
///* CPRI信号送信指示(メーカMT接続強制解放応答)								*/
//typedef struct
//{
//	CMT_CPRISIGSND_INF		cprisnd_inf;		/* CPRI信号送信情報部		*/
//												/* CPRI信号データ部			*/
//	CMT_CPRIF_MKMTFORRELRES	cpridat_mkmtforrelres;
//} CMT_TSKIF_CPRISND_MKMTFORRELRES;

/****************************************************************************/
/* MESSAGE :スレッド間メッセージID取得用									*/
/****************************************************************************/
typedef struct
{
	T_MSGHEAD				head;				/* Message common header	*/
}CMT_TSKIF_MSGID_GET;

///****************************************************************************/
///* MESSAGE :スレッド起動完了通知											*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					result;				/* 結果						*/
//	USHORT					dmy;				/* ダミー					*/
//}CMT_TSKIF_THRSUPCMP_NTC;
//
///****************************************************************************/
///* MESSAGE :スレッド初期化要求												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//}CMT_TSKIF_THRINI_REQ;
//
///****************************************************************************/
///* MESSAGE :スレッド初期化応答												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					result;				/* 結果						*/
//	USHORT					dmy;				/* ダミー					*/
//}CMT_TSKIF_THRINI_RSP;
//
///****************************************************************************/
///* MESSAGE :全スレッド初期化完了通知										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//}CMT_TSKIF_ALLTHRINICMP_NTC;
//
///****************************************************************************/
///* MESSAGE :全プロセス初期化完了通知										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//}CMT_TSKIF_ALLPRCINICMP_NTC;

/****************************************************************************/
/* MESSAGE :タイムアウト発生通知											*/
/****************************************************************************/
/** 
 * @brief   タイムアウト発生通知
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	bpf_ru_hrtm_key_inf_t	key_inf;			/**< key information		*/
	UINT					timerId;			/**< Timer ID				*/
}CMT_TSKIF_TIMOUTNTC;

///****************************************************************************/
///* MESSAGE :データリンク設定要求(CMT_TSKIF_L2ESTREQ)      */
///****************************************************************************/
//typedef struct
//{
// T_MSGHEAD  head;
// USHORT   link_num;  /* CPRI番号     */
// UCHAR   dummy1;   /* ダミー領域 1   */
// UCHAR   dummy2;   /* ダミー領域 2   */
//}CMT_TSKIF_L2ESTREQ;
//
///****************************************************************************/
///* MESSAGE :データリンク設定確認(CMT_TSKIF_L2ESTCNF)      */
///****************************************************************************/
//typedef struct
//{
// T_MSGHEAD   head;
// USHORT   link_num;
// UCHAR   dummy1;
// UCHAR   dummy2;
//}CMT_TSKIF_L2ESTCNF;
//
///****************************************************************************/
///* MESSAGE :L3リンク制御確立通知(CMT_TSKIF_L3CNTESTNTC)      */
///****************************************************************************/
//typedef struct
//{
// T_MSGHEAD  head;     /* タスク共通ヘッダ   */
// USHORT    cpri_no;    /* CPRIリンク番号   */
// USHORT    dummy;     /* ダミー領域    */
//}CMT_TSKIF_L3CNTESTNTC;
//
///****************************************************************************/
///* MESSAGE :L2 DEACT指示													*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;		/* CPRI番号					*/
//#if defined(OPT_CPRI_L2HARD)
//	USHORT			deActSts;		/* DEACT状態(0:DEACT(設定)/1:ACT(解除))	*/
//#else
//	UCHAR			dummy1;			/* ダミー領域	1			*/
//	UCHAR			dummy2;			/* ダミー領域	2			*/
//#endif
//}CMT_TSKIF_L2DEACTIND;
//
///****************************************************************************/
///* MESSAGE :データリンク解放要求(CMT_TSKIF_L2RELREQ)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;		/* CPRI番号					*/
//	UCHAR			dummy1;			/* ダミー領域	1			*/
//	UCHAR			dummy2;			/* ダミー領域	2			*/
//}CMT_TSKIF_L2RELREQ;
//
///****************************************************************************/
///* MESSAGE :自受信ビジー発生通知											*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					linknum;			/* link number */
//	USHORT					buftyp;				/* buffer type */
//}CMT_TSKIF_L2BSY;
//
///****************************************************************************/
///* MESSAGE :自受信ビジー解除通知											*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					linknum;			/* link number */
//	USHORT					buftyp;				/* buffer type */
//}CMT_TSKIF_L2BSYCLR;
//
///****************************************************************************/
///* MESSAE :DATETIME                                                         */
///****************************************************************************/
//typedef struct
//{
//	USHORT			year  ;					/* 年							*/
//	UCHAR			month ;					/* 月							*/
//	UCHAR			day   ;					/* 日							*/
//	UCHAR			hour  ;					/* 時							*/
//	UCHAR			min   ;					/* 分							*/
//	UCHAR			sec   ;					/* 秒							*/
//	UCHAR			msec  ;					/* ミリ秒						*/
//
//} DATETIME_T ;
//
///***************************************************************************/
///* MESSAGE: LAPB head                                                      */
///***************************************************************************/
//typedef struct
//{
//	USHORT							link_num;	/* CPRI番号					*/
//	UCHAR							dummy1;		/* ダミー領域	1			*/
//	UCHAR							dummy2;		/* ダミー領域	2			*/
//	struct CMT_TSKIF_L1DATRCVNTC	*i_link;	/* I queue pointer			*/
//	UINT							length;		/* buffer length			*/
//	UINT							disp;		/* buffer offset sizes		*/
//	UCHAR							Padding1;	/* Padding1					*/
//	UCHAR							Padding2;	/* Padding2					*/
//	UCHAR							Address;	/* HDLC frame address area	*/
//	UCHAR							Control;	/* HDLC frame control area	*/
//}CMT_LAPB_HEAD;
//
///****************************************************************************/
///* MESSAGE: CPRI sending message indication                                 */
///* MESSAGE ID:  0x52060004                                                  */
///****************************************************************************/
//typedef struct
//{
//	DATETIME_T        day_time;
//	CMT_LAPB_HEAD     lapb_head;
//
//}CPRI_DATA_SND_IND_MON;
///***************************************************************************/
///* MESSAGE :CPRI recieving message indication                              */
///* MESSAGE ID: 0x52070004                                                  */
///***************************************************************************/
//typedef struct
//{
//	DATETIME_T    day_time;
//	CMT_LAPB_HEAD     lapb_head;
//
//}CPRI_DATA_RCV_IND_MON;
//
///*****************************************************************************/
///*    MESSAGE :L1 send message requirement                                   */
///*****************************************************************************/
//typedef struct
//{
//	CPRI_DATA_SND_IND_MON       snd_msg_ind;
//}CMT_TSKIF_L1MONDATSNDNTC;
//
///****************************************************************************/
///*   MESSAGE :L1 recieve message notification                               */
///****************************************************************************/
//typedef struct
//{
//	CPRI_DATA_RCV_IND_MON  rcv_msg_ind;
//}CMT_TSKIF_L1MONDATRCVNTC;
//
//
///****************************************************************************/
///* MESSAGE :L1データ受信通知(CMT_TSKIF_L1DATRCVNTC)							*/
///****************************************************************************/
//typedef struct CMT_TSKIF_L1DATRCVNTC
//{
//	USHORT							link_num;	/* CPRI番号					*/
//	UCHAR							dummy1;		/* ダミー領域	1			*/
//	UCHAR							dummy2;		/* ダミー領域	2			*/
//	struct CMT_TSKIF_L1DATRCVNTC	*i_link;	/* I queue pointer			*/
//	UINT							length;		/* buffer length			*/
//	UINT							disp;		/* buffer offset sizes		*/
//	UCHAR							Padding1;	/* Padding1					*/
//	UCHAR							Padding2;	/* Padding2					*/
//	UCHAR							Address;	/* HDLC frame address area	*/
//	UCHAR							Control;	/* HDLC frame control area	*/
//}CMT_TSKIF_L1DATRCVNTC;
//
///****************************************************************************/
///* MESSAGE :L1データ送信完了通知											*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;		/* CPRI番号					*/
//	UCHAR			dummy1;			/* ダミー領域	1			*/
//	UCHAR			dummy2;			/* ダミー領域	2			*/
//}CMT_TSKIF_L1DATCNF;
//
///****************************************************************************/
///* MESSAGE :TX送信異常通知(CMT_TSKIF_TXABNORMAL)							*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT  			linkno;
//	USHORT				dummy;
//}CMT_TSKIF_TXABNORMAL;
//
///****************************************************************************/
///* MESSAGE :L1データ送信指示												*/
///****************************************************************************/
//typedef struct
//{
//	USHORT						link_num;		/* CPRI番号					*/
//	UCHAR						dummy1;			/* ダミー領域	1			*/
//	UCHAR						dummy2;			/* ダミー領域	2			*/
//	struct CMT_TSKIF_L1DATIND	*i_link;		/* I queue pointer			*/
//	UINT						length;			/* buffer length			*/
//	UINT						disp;			/* buffer offset sizes		*/
//	UCHAR						Padding1;		/* Padding1					*/
//	UCHAR						Padding2;		/* Padding2					*/
//	UCHAR						Address;		/* HDLC frame address area	*/
//	UCHAR						Control;		/* HDLC frame control area	*/
//}CMT_TSKIF_L1DATIND;
//
///****************************************************************************/
///* MESSAGE :データリンク設定表示(CMT_TSKIF_L2ESTIND)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L2ESTIND;
//
///****************************************************************************/
///* MESSAGE :データリンク設定中表示(CMT_TSKIF_L2ESTINGIND)					*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L2ESTINGIND;
//
///****************************************************************************/
///* MESSAGE :データリンク解放確認(CMT_TSKIF_L2RELCNF)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L2RELCNF;
//
///****************************************************************************/
///* MESSAGE :データリンク解放表示(CMT_TSKIF_L2RELIND)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L2RELIND;
//
///****************************************************************************/
///* MESSAGE :L2 STOP指示														*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L2STPREQ;
//
///****************************************************************************/
///* MESSAGE :L3デバッグ開始指示												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L3DBGSTAIND;
//
///****************************************************************************/
///* MESSAGE :L3デバッグ停止指示												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT			link_num;
//	UCHAR			dummy1;
//	UCHAR			dummy2;
//}CMT_TSKIF_L3DBGSTPIND;
//
///****************************************************************************/
///* MESSAGE :CPRI-MT運用開始指示												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				mode;				/* Mode(0:monitor; 1:simulator)	*/
//	USHORT				dummy;				/* Dummy						*/
//	USHORT 				linkflg[CMD_LINK_NUM_T];/* Link flag(1:care;0:not care)*/
//}CMT_TSKIF_CPRIMTSTRIND;
//
///****************************************************************************/
///* MESSAGE :CPRI-MT運用停止指示												*/
///****************************************************************************/
///* メーカMT管理 --> TXIOCS,RXIOCS	*/
//typedef struct
//{
//	T_MSGHEAD		head;					/* Message common header	*/
//	USHORT			mode;					/* Mode(0:monitor/1:simulator)	*/
//	USHORT			dummy;					/* Dummy						*/
//	USHORT 			linkflg[CMD_LINK_NUM_T]; /* Link flag(1:care;0:not care)*/
//}CMT_TSKIF_CPRIMTSTPIND;
//
///****************************************************************************/
///* MESSAGE :CPRIデバッグ機能開始要求										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				linkno;					/* link番号					*/
//	USHORT				simmode;				/* CPRI simulator mode		*/
//}CMT_TSKIF_CPRIDBGSTAREQ;
//
///****************************************************************************/
///* MESSAGE :CPRIデバッグ機能開始応答										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				linkno;					/* link番号					*/
//	USHORT				result;					/* 応答結果					*/
//}CMT_TSKIF_CPRIDBGSTARSP;
//
///****************************************************************************/
///* MESSAGE :CPRIデバッグ機能停止要求										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no_range;			/* CPRI simulator link
//															specify range	*/
//	USHORT				linkno;					/*  link番号				*/
//}CMT_TSKIF_CPRIDBGSTPREQ;
//
///****************************************************************************/
///* MESSAGE :CPRIデバッグ機能停止応答										*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				linkno;					/* link番号					*/
//	USHORT				result;			        /* 応答結果					*/
//}CMT_TSKIF_ONELINKRSP;
//
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					linknum;			/* total link number		*/
//	CMT_TSKIF_ONELINKRSP	stopresult[CMD_LINK_NUM_T];		/* 応答結果					*/
//	USHORT					dummy;				/* ダミー領域				*/
//}CMT_TSKIF_CPRIDBGSTPRSP;
//
///****************************************************************************/
///* MESSAGE :L1データ受信(運用状態監視)通知									*/
///****************************************************************************/
//typedef struct CMT_TSKIF_L1MDATRCVNTC
//{
//	USHORT						link_num;		/* CPRI番号					*/
//	UCHAR						dummy1;			/* ダミー領域	1			*/
//	UCHAR						dummy2;			/* ダミー領域	2			*/
//	struct CMT_TSKIF_L1MDATRCVNTC	*i_link;	/* I queue pointer			*/
//	UINT						length;			/* buffer length			*/
//	UINT						disp;			/* buffer offset sizes		*/
//	UCHAR						Padding1;		/* Padding1					*/
//	UCHAR						Padding2;		/* Padding2					*/
//	UCHAR						Address;		/* HDLC frame address area	*/
//	UCHAR						Control;		/* HDLC frame control area	*/
//}CMT_TSKIF_L1MDATRCVNTC;
//
///****************************************************************************/
///* MESSAGE :L1データ送信(運用状態監視)通知									*/
///****************************************************************************/
//typedef struct CMT_TSKIF_L1MDATSNDREQ
//{
//	USHORT						link_num;		/* CPRI番号					*/
//	UCHAR						dummy1;			/* ダミー領域	1			*/
//	UCHAR						dummy2;			/* ダミー領域	2			*/
//	struct CMT_TSKIF_L1MDATSNDREQ    *i_link;	/* I queue pointer			*/
//	UINT				length;					/* buffer length			*/
//	UINT				disp;					/* buffer offset sizes		*/
//	UCHAR				Padding1;				/* Padding1					*/
//	UCHAR				Padding2;				/* Padding2					*/
//	UCHAR				Address;				/* HDLC frame address area	*/
//	UCHAR				Control;				/* HDLC frame control area	*/
//}CMT_TSKIF_L1MDATSNDREQ;
//
///****************************************************************************/
///* MESSAGE :CPRI運用状態監視開始指示										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//#if defined(OPT_CPRI_L2HARD)
//	USHORT 			linkflg[CMD_LINK_NUM_T];	/* Link flag(1:開始)*/
//#endif
//} CMT_TSKIF_CPRIMONSTRIND;
//
///****************************************************************************/
///* MESSAGE :CPRI運用状態監視停止指示										*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI 番号				*/
//	USHORT				dummy;					/* Dummy					*/
//} CMT_TSKIF_CPRIMONSTPIND;
//
///****************************************************************************/
///* MESSAGE :Active切替指示													*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				chg_trg;				/* 切替トリガー				*/
//	USHORT				chglink_no;				/* 切替リンク番号			*/
//}CMT_TSKIF_ACTCHGIND;
//
/****************************************************************************/
/* MESSAGE :CPRIリンク状態変化通知											*/
/****************************************************************************/
/**
 * @brief   CPRIリンク状態変化通知
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< CPRI番号				*/
	USHORT					link_sts;			/**< リンク状態				*/
}CMT_TSKIF_CPRSTACHGNTC;

/****************************************************************************/
/* MESSAGE :CPRIリンク断通知												*/
/****************************************************************************/
/**
 * @brief   CPRIリンク断通知
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< CPRI番号				*/
	USHORT					link_sts;			/**< リンク状態				*/
}CMT_TSKIF_CPRDSCNTC;

///****************************************************************************/
///* MESSAGE :Active切替結果通知(CMT_TSKIF_ACTCHGRLTNTC)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				chg_result;				/* 切替結果					*/
//	USHORT				chg_trg;				/* 切替トリガー				*/
//	USHORT				chglink_no;				/* リンク切替元番号 		*/
//	USHORT				newactlink_no;			/* 新activeリンク番号		*/
//}CMT_TSKIF_ACTCHGRLTNTC;
//
///****************************************************************************/
///* MESSAGE :Negative通知(CMT_TSKIF_NEGNTC)									*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				oldactlink_no;			/* 旧activeリンク番号		*/
//	USHORT				actdsc_inf;		/* active disconnection detail info */
//}CMT_TSKIF_NEGNTC;
//
///****************************************************************************/
///* MESSAGE :Active切替禁止要求(CMT_TSKIF_ACTSWFORBREQ)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				ampno;					/* AMP番号					*/
//	USHORT				reqtype;				/* request type				*/
//}CMT_TSKIF_ACTSWFORBREQ;
//
///****************************************************************************/
///* MESSAGE :Active切替禁止応答(CMT_TSKIF_ACTSWFORBRSP)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				result;					/* 応答結果					*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_ACTSWFORBRSP;
//
///****************************************************************************/
///* MESSAGE :CPRIリンクstate通知(CMD_TSKIF_CPRISTANTC)						*/
///****************************************************************************/
//typedef struct
//{
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				reequ_no;				/* RE装置番号				*/
//	USHORT				cpriconf;				/* linkCPRI使用/未使用情報	*/
//	USHORT				cprista;				/* CPRI state				*/
//	USHORT				hdlcsta;				/* HDLC state				*/
//	USHORT				L2abnminfo;				/* L2 link 異常情報			*/
//	USHORT				hcsta;					/* ヘルスチェック情報		*/
//	USHORT				recomactinfo;	/* RE共通部リンクactive/negative情報*/
//	USHORT				actswforbflg;		/* RE共通部active切替禁止flag	*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_CPRISTA;
//
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//  	DATETIME_T			timestamp;				/* time stamp				*/
//	CMT_TSKIF_CPRISTA	cprilnksta[CMD_LINK_NUM_T];  /* max 12 CPRI link state */
//}CMT_TSKIF_CPRISTANTC;
//
///****************************************************************************/
///* MESSAGE :RE強制停止通知(CMT_TSKIF_RECMPSTPNTC)							*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_RECMPSTPNTC;
//
///****************************************************************************/
///* MESSAGE :Active設定要求													*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_ACTSETREQ;
//
///****************************************************************************/
///* MESSAGE :Active設定応答													*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				result_code;			/* 結果コード				*/
//	USHORT				activelink_no;			/* activeリンク番号			*/
//}CMT_TSKIF_ACTSETRSP;
//
///****************************************************************************/
///* MESSAGE :二次リセット要求												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_2NDRST_REQ;
//
///****************************************************************************/
///* MESSAGE :二次リセット応答												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_2NDRST_RSP;
//
///****************************************************************************/
///* MESSAGE :RE起動完了通知(CMT_TSKIF_RESRTCMPNTC)							*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRIリンク番号			*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_RESRTCMPNTC;
//
///****************************************************************************/
///* MESSAGE :全RE強制停止指示(CMT_TSKIF_REALLRSTNTC)							*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	UINT				amp_no;					/* AMP番号					*/
//}CMT_TSKIF_REALLRSTNTC;
//
///****************************************************************************/
///* MESSAGE :REファイル更新要求(CMT_TSKIF_REFILNEWREQ)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				sdm_side;				/* SDM Side     */
//	USHORT				kind;					/* download kind (0:RE startup/1:API) */
//}CMT_TSKIF_REFILNEWREQ;
//
///****************************************************************************/
///* MESSAGE :REファイル更新応答(CMT_TSKIF_REFILNEWRES)						*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				result_code;			/* 結果コード				*/
//	USHORT				reset_jdg;				/* REリセット要否			*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_REFILNEWRES;
//
///****************************************************************************/
///* MESSAGE :REシステムパラメータ更新要求(CMT_TSKIF_RESYSNEWREQ)				*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				dummy;					/* ダミー領域				*/
//}CMT_TSKIF_RESYSNEWREQ;
//
///****************************************************************************/
///* MESSAGE :REシステムパラメータ更新応答(CMT_TSKIF_RESYSNEWRES)				*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				result_code;			/* 結果コード				*/
//    USHORT              primary_sys;            /* the primary system of RE */
//}CMT_TSKIF_RESYSNEWRES;
//
///****************************************************************************/
///* MESSAGE :RE更新停止指示(CMT_TSKIF_RENEWSTPNTC)				*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT				cpri_no;				/* CPRI番号					*/
//	USHORT				fact;					/* fact */
//}CMT_TSKIF_RENEWSTPNTC;
//
///****************************************************************************/
///* MESSAGE :OCMイベント受信通知												*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	bpf_rm_uprm_event		ocm_evt;			/* OCMイベント */
//}CMT_TSKIF_OCMEVTRCV_NTC;
//
/****************************************************************************/
/* MESSAGE :二次リセットプロシージャ実行指示                                */
/****************************************************************************/
/**
 * @brief   二次リセットプロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
}CMT_TSKIF_2NDRSTPRCIND;
/****************************************************************************/
/* MESSAGE :RE時刻設定プロシージャ実行指示                                  */
/****************************************************************************/
/**
 * @brief   RE時刻設定プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RETIMSETPRCIND;
/****************************************************************************/
/* MESSAGE :REシステムパラメータ更新プロシージャ実行指示                    */
/****************************************************************************/
/**
 * @brief   REシステムパラメータ更新プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_REPRMPRCIND;
/****************************************************************************/
/* MESSAGE :REファイル更新プロシージャ実行指示                              */
/****************************************************************************/
/**
 * @brief   REファイル更新プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_REFILPRCIND;
/****************************************************************************/
/* MESSAGE :Toffset値報告プロシージャ実行指示                               */
/****************************************************************************/
/**
 * @brief   Toffset値報告プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_TOFVALPRCIND;
/****************************************************************************/
/* MESSAGE :Active設定プロシージャ実行指示                                  */
/****************************************************************************/
/**
 * @brief   Active設定プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_ACTSETPRCIND;
/****************************************************************************/
/* MESSAGE :RE装置構成情報報告プロシージャ実行指示                          */
/****************************************************************************/
/**
 * @brief   RE装置構成情報報告プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_REEQPINFPRCIND;
/****************************************************************************/
/* MESSAGE :REスロット情報報告プロシージャ実行指示                          */
/****************************************************************************/
/**
 * @brief   REスロット情報報告プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RESLTINFPRCIND;
/****************************************************************************/
/* MESSAGE :RE状態報告確プロシージャ実行指示                                */
/****************************************************************************/
/**
 * @brief   RE状態報告プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RESTSPRCIND;
/****************************************************************************/
/* MESSAGE :RE PORT状態報告プロシージャ実行指示                             */
/****************************************************************************/
/**
 * @brief   RE PORT状態報告プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_REPRTSTSPRCIND;
/****************************************************************************/
/* MESSAGE :REカード状態報告(セクタ部)プロシージャ実行指示                  */
/****************************************************************************/
/**
 * @brief   REカード状態報告(セクタ部)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RECRDSSCPRCIND;
/****************************************************************************/
/* MESSAGE :REカード状態報告(共通部)プロシージャ実行指示                    */
/****************************************************************************/
/**
 * @brief   REカード状態報告(共通部)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RECRDSCMPRCIND;
/****************************************************************************/
/* MESSAGE :REキャリア状態報告プロシージャ実行指示                          */
/****************************************************************************/
/**
 * @brief   REキャリア状態報告プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RECRRSTSPRCIND;
/****************************************************************************/
/* MESSAGE :MTアドレス設定(FLD-MT)プロシージャ実行指示                      */
/****************************************************************************/
/**
 * @brief   MTアドレス設定(FLD-MT)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_MTSETFPRCIND;
/****************************************************************************/
/* MESSAGE :MTアドレス設定(SV-MT)プロシージャ実行指示                       */
/****************************************************************************/
/**
 * @brief   MTアドレス設定(SV-MT)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_MTSETSPRCIND;
/****************************************************************************/
/* MESSAGE :RE起動完了プロシージャ実行指示                                  */
/****************************************************************************/
/**
 * @brief   RE起動完了プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RESCMPPRCIND;
/****************************************************************************/
/* MESSAGE :REリセットプロシージャ実行指示                                  */
/****************************************************************************/
/**
 * @brief   REリセットプロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_RERSTPRCIND;
/****************************************************************************/
/* MESSAGE :RE強制リセット設置プロシージャ実行指示                          */
/****************************************************************************/
/**
 * @brief   RE強制リセット設置プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_REFRCRSTSETPRCIND;
/****************************************************************************/
/* MESSAGE :RE強制リセットプロシージャ実行指示                              */
/****************************************************************************/
/**
 * @brief   RE強制リセットプロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_REFRCRSTPRCIND;
/****************************************************************************/
/* MESSAGE :強制停止プロシージャ実行指示                                    */
/****************************************************************************/
/**
 * @brief   強制停止プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< 結果					*/
	USHORT					dmy;				/**< ダミー					*/
	T_RRH_RESTOP_FACTOR     stopFactor;         /**< 強制停止要因           */
}CMT_TSKIF_FRCSTPPRCIND;
/****************************************************************************/
/* MESSAGE :TRX設定(自律)プロシージャ実行指示                               */
/****************************************************************************/
/**
 * @brief   TRX設定(自律)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< CPRI番号				*/
	USHORT					crr_no;				/**< キャリア番号			*/
}CMT_TSKIF_TXSAPRCIND;
/****************************************************************************/
/* MESSAGE :TRX解放(自律)プロシージャ実行指示                               */
/****************************************************************************/
/**
 * @brief   TRX解放(自律)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< CPRI番号				*/
	USHORT					crr_no;				/**< キャリア番号			*/
}CMT_TSKIF_TXRAPRCIND;
/****************************************************************************/
/* MESSAGE :無変調キャリアON/OFF指定(自律)プロシージャ実行指示              */
/****************************************************************************/
/**
 * @brief   無変調キャリアON/OFF指定(自律)プロシージャ実行指示
 */
typedef struct
{
	T_MSGHEAD				head;				/**< Message common header	*/
	USHORT					cpri_no;			/**< CPRI番号				*/
	USHORT					crr_no;				/**< キャリア番号			*/
	USHORT					onoff;				/**< ON/OFF指定				*/
	USHORT					dmy;				/**< ダミー					*/
}CMT_TSKIF_NMCAPRCIND;
///****************************************************************************/
///* MESSAGE : TRA file download request to OAM */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;
//	CHAR					refile[CMD_NUM20];
//	USHORT					makertype;
//	USHORT					version;
//	USHORT					sdmside;
//	USHORT					dmy;
//}CMT_TSKIF_TRAFILDLMREQ;
//
///****************************************************************************/
///* MESSAGE : TRA file download response from OAM */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;
//	USHORT					okng;
//	USHORT					makertype;
//	USHORT					version;
//	USHORT					sdmside;
//	USHORT					bufno;
//	USHORT					dmy;
//}CMT_TSKIF_TRAFILDLMRES;
//
///****************************************************************************/
///*!
// * @brief MESSAGE :LAPB N/E切替開始応答(CMT_TSKIF_XXX)
// * @note 起動要因:0x0908
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//	USHORT				cprinum;				/*!< total link number		*/
//	USHORT				linkno[CMD_LINK_NUM_T];				/*!< 再起動link番号			*/
//	USHORT				dummy;					/*!< ダミー領域				*/
//}CMT_TSKIF_LPBNESWSTRRSP;
//
///****************************************************************************/
///*!
// * @brief MESSAGE :N/E間データ引継ぎ完了通知(CMT_TSKIF_NEDATCMPNTC)
// * @note 起動要因:0x090A
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//	USHORT				prockind;				/*!< 処理種別					*/
//	USHORT				dummy;					/*!< ダミー領域				*/
//}CMT_TSKIF_NEDATCMPNTC;
//
///****************************************************************************/
///*!
// * @brief MESSAGE :N/E switch complete notice(CMT_TSKIF_NECHGCMPNTC)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//	USHORT				prockind;				/*!< 処理種別					*/
//	USHORT				dummy;					/*!< ダミー領域				*/
//}CMT_TSKIF_NECHGCMPNTC;
//
///****************************************************************************/
///*!
// * @brief MESSAGE CPRI link recover complete notice(CMT_TSKIF_CPRIRCVCMPNTC)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//}CMT_TSKIF_CPRIRCVCMPNTC;
//
///****************************************************************************/
///*!
// * @brief MESSAGE NE switch start response(CMT_TSKIF_NECHGSRTRSP)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//}CMT_TSKIF_NECHGSRTRSP;
//
///****************************************************************************/
///*!
// * @brief MESSAGE CPRI link restart notice(CMT_TSKIF_CPRRESTRNTC)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//}CMT_TSKIF_CPRRESTRNTC;
//
///****************************************************************************/
///*!
// * @brief MESSAGE NE switch start request(CMT_TSKIF_NECHGSRTREQ)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//}CMT_TSKIF_NECHGSRTREQ;
//
//
///****************************************************************************/
///*!
// * @brief MESSAGE LAPB NE switch start request(CMT_TSKIF_LBNECHGSRTREQ)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//}CMT_TSKIF_LBNECHGSRTREQ;
//
//
///****************************************************************************/
///*!
// * @brief MESSAGE NE data succession start notice (CMT_TSKIF_NEDATSRTNTC)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/*!< Message common header	*/
//	USHORT				prockind;				/*!< 処理種別					*/
//	USHORT				dummy;					/*!< ダミー領域				*/
//}CMT_TSKIF_NEDATSRTNTC;
//
//
///****************************************************************************/
///* MESSAGE :CPRI Link state notification(Betwwen PC and TRIF)				*/
///****************************************************************************/
//typedef struct
//{
//	USHORT		cpri_no;				/* CPRI番号					*/
//	USHORT		reequ_no;				/* RE装置番号				*/
//	USHORT		cpriconf;				/* linkCPRI使用/未使用情報	*/
//	USHORT		cprista;				/* CPRI state				*/
//	USHORT		hdlcsta;				/* HDLC state				*/
//	USHORT		L2abnminfo;				/* L2 link 異常情報			*/
//	USHORT		hcsta;					/* ヘルスチェック情報		*/
//	USHORT		recomactinfo;			/* RE共通部リンクactive/negative情報 */
//	USHORT		actswforbflg;			/* RE共通部active切替禁止flag	*/
//	USHORT		dummy;					/* ダミー領域				*/
//}CMT_CRDIF_CPRISTA;
//
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	DATETIME_T			timestamp;				/* time stamp				*/
//	CMT_CRDIF_CPRISTA	cprilnksta[CMD_LINK_NUM_PC];  /* max 12 CPRI link state */
//}CMT_CRDIF_CPRISTANTC;
///****************************************************************************/
///* MESSAGE :CPRI MT start request											*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;					/* Message common header		*/
//	USHORT					mode;					/* Mode(0:monitor/1:simulator)	*/
//	USHORT					dummy;					/* Dummy						*/
//	USHORT 				linkflg[CMD_LINK_NUM_T];    /* Link flag(1:care;0:not care)*/
//}CMT_TSKIF_CPRIMTSTPREQ;
///****************************************************************************/
///* MESSAGE :CPRI MT Debug start request  									*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					linkno;				/* Link number				*/
//	USHORT					simmode;			/* CPRI simulator mode		*/
//}CMT_TSKIF_CPRIDBGSTRREQ;
//
///****************************************************************************/
///* MESSAGE :CPRI MT Debug Stop request  									*/
///****************************************************************************/
//typedef struct
//{
//	T_MSGHEAD				head;		/* Message common header	*/
//	USHORT 				linkrange;		/* Link range	(1:single link;	2:all links)*/
//	USHORT 				linkno;			/* Link no				*/
//}CMT_CRDIF_CPRIDBGSTPREQ;
//
///********************************************************************/
///* CPRI monitor start request										*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD	head;						/* Message common header     */
//	USHORT 		linkflg[CMD_LINK_NUM_T];	/* 0 : Unchange		1 : Start*/
//}CMT_TSKIF_CPRIMONSTRREQ;
//
///********************************************************************/
///* CPRI monitor stop request										*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD		head;					/* Message common header */
//	USHORT			cpri_no;				/* CPRI Number			 */
//	USHORT			dummy;					/* Dummy			  	 */
//} CMT_TSKIF_CPRIMONSTPREQ;
//
///********************************************************************/
///* Interruption notice												*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD		head;					/*!< Message common header */
//	UINT			main_int_val;			/*!< Interruption factor register value	 */
//	UINT			dmac_int_val;			/*!< DMAC interruption factor register value	 */
//											/*!< If DMAC_INT bit in main_int_val is OFF, this is D.C. */
//} CMT_TSKIF_INTERUPNTC;
//
//
///********************************************************************/
///* RE file download permission request								*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD		head;					/* Message common header */
//	USHORT			cpri_no;				/* CPRI Number			 */
//	USHORT			dummy;					/* Dummy			  	 */
//} CMT_TSKIF_REDLPERMITREQ;
//
///********************************************************************/
///* RE file download permission response								*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD		head;					/* Message common header */
//	USHORT			cpri_no;				/* CPRI Number			 */
//	USHORT			dummy;					/* Dummy			  	 */
//} CMT_TSKIF_REDLPERMITRSP;
//
///********************************************************************/
///* MT data sending permission request								*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD		head;					/* Message common header */
//	USHORT			cpri_no;				/* CPRI Number			 */
//	USHORT			dummy;					/* Dummy			  	 */
//} CMT_TSKIF_MTDATAPERMITREQ;
//
///********************************************************************/
///* MT data sending permission response								*/
///********************************************************************/
//typedef struct
//{
//	T_MSGHEAD		head;					/* Message common header */
//	USHORT			cpri_no;				/* CPRI Number			 */
//	USHORT			dummy;					/* Dummy			  	 */
//} CMT_TSKIF_MTDATAPERMITRSP;
//
//
///* ======================================================================== */
///* テーブル構造体定義														*/
///* ======================================================================== */
//
///****************************************************************************/
///* CPRIリンクコンフィッグテーブル(CRT_CPRICONFTBL)							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT		actswforbflg;					/* active切替禁止flag		*/
//	USHORT		clscause;						/* 「RE共通部確立」要因		*/
//	USHORT		actlnk;							/* 既存activeリンク			*/
//	USHORT		settingactlnk;					/* 設定中のactiveリンク		*/
//	USHORT		origactlnk;						/* 旧activeリンク			*/
//	USHORT		clsseq;							/* 「RE共通部確立」sequence	*/
//	UINT		actdscstbtmid;		/* active断「カード安定化時間」timer ID	*/
//	UINT		clscttmid;						/* CLS-CT timer ID			*/
//	USHORT		actdscstbflg;		/* active断「カード安定化時間」実行flag	*/
//	USHORT		ampcprinum;			/* total CPRI number in the RE			*/
//	USHORT		cpri_no[CMD_CPRINO_MAX];		/* CPRI番号 				*/
//}CMT_CLSINFO;
//
//typedef struct
//{
//	USHORT		cpri_no;						/* CPRI番号					*/
//	USHORT		retype;							/* RE type					*/
//	USHORT		cpriconf;						/* linkCPRI使用/未使用情報	*/
//	USHORT		actendisflg;		/* active link enable/disable flag		*/
//	USHORT		clsrc;				/* 「RE共通部確立」retry time counter	*/
//	USHORT		AutoRstCtrlFlg;				/* Auto RESET Control Flag		*/
//}CMT_CPRICONFINFO;
//
//typedef struct
//{
//	USHORT			selfcardpos;							/* 自カード実装位置情報		*/
//	USHORT			dummy;									/* ダミー領域				*/
//	CMT_CLSINFO		clsinfo[D_TCM_ACTIVE_LINK_GROUP_NUM];	/* RE共通部リンク情報		*/
//	CMT_CPRICONFINFO	cpriconfinfo[CMD_LINK_NUM_T];		/* CPRI config info	*/
//}CMT_CPRICONFTBL;
//
///****************************************************************************/
///* CPRI state共通管理テーブル(CRT_CPRILNKSTATBL)							*/
///****************************************************************************/
//typedef struct
//{
//	USHORT		cpri_no;						/* CPRI番号					*/
//	USHORT		cprisimflg;						/* CPRI Simulator use flag	*/
//	USHORT		cprilnkstano;					/* CPRI link state number	*/
//	USHORT		cprista;						/* CPRI state				*/
//	USHORT		hdlcsta;						/* HDLC idle/active flag	*/
//	USHORT		stafflg;						/* stateF flag				*/
//	USHORT		L2abnminfo;						/* L2 リンク異常情報		*/
//	USHORT		hcsta;							/* L3 health check state	*/
//	USHORT		restaupflg;						/* 再起動flag				*/
//	USHORT		cpridscstbflg;					/* CPRI断「カード状態安定化時間」実行flag	*/
//	UINT		cpridsctmid;					/* CPRI断「カード状態安定化時間」timer ID	*/
//	USHORT		hccntestflag;					/* L3リンク制御確立flag		*/
//	USHORT		statec_flg;						/* State C flag				*/
//	USHORT		irq3state;						/* State report from IRQ3	*/
//	USHORT		state_for_rec;       			/* cpri state (A to L3 OK)	*/
//	USHORT		L1dscstbflg;					/* L1断「カード状態安定化時間」実行flag	*/
//	USHORT		L1dsctmid;             			/* L1断「カード状態安定化時間」timer ID */
//	USHORT		REdlflg;             			/* RE file download permission request received flag */
//	USHORT		Mtdataflg;             			/* MT data sending permission request received flag */
//}CMT_CPRILNKSTAINFO;
//
//typedef struct
//{
//	USHORT		cprimonflg;		/* CPRI monitor実行flag						*/
//	USHORT		estendisflg[CMD_LINK_NUM_T];	 /* CPRI link establish enable/disable flag	*/
//								/* CPRI link state情報						*/
//	USHORT		cmuxrstinfo ;   /* CPRI-MUX含むリセット情報(1:CPRI-MUX含むリセット,0:以外のリセット) */
//	CMT_CPRILNKSTAINFO	cprilnkstainfo[CMD_LINK_NUM_T];
//
//}CMT_CPRILNKSTATBL;
//
///****************************************************************************/
///* CPRI NE切替情報管理テーブル(CMT_CPRINECHGINFMNG_TBL)						*/
///****************************************************************************/
//typedef struct									/* RE共通リンク情報型		*/
//{
//	USHORT		clscause;						/* 「RE共通部確立」要因		*/
//	USHORT		actlnk;							/* 既存activeリンク			*/
//	USHORT		settingactlnk;					/* 設定中のactiveリンク		*/
//	USHORT		origactlnk;						/* 旧activeリンク			*/
//	USHORT		clsseq;							/* 「RE共通部確立」sequence	*/
//	USHORT		actdscstbflg;					/* active断「カード安定化時間」実行flag	*/
//}CMT_RECLS_INF;
//
//typedef struct									/* CPRIコンフィグ情報型		*/
//{
//	USHORT		actendisflg;					/* active link enable/disable flag	*/
//	USHORT		clsrc;							/* 「RE共通部確立」retry time counter	*/
//}CMT_CPRICONF_INF;
//
//typedef struct									/* CPRIリンクステート情報型	*/
//{
//	USHORT		cprilnkstano;					/* CPRI link state number	*/
//	USHORT		cprista;						/* CPRI state				*/
//	USHORT		restaupflg;						/* 再起動flag				*/
//	USHORT		cpridscstbflg;					/* CPRI断「カード状態安定化時間」実行flag	*/
//	USHORT		L1dscstbflg;					/* L1断「カード状態安定化時間」実行flag	*/
//	USHORT		state_for_rec;             		/* cpri state (A to L3 OK)	*/
//}CMT_CPRILNKSTA_INF;
//
//typedef struct									/* レジスタ情報型	*/
//{
//	UINT		cprierr[CMD_NUM2];	/*	CPRI link ERR REL/ERR SET register	value	*/
//	UINT		t4set;							/*	T4 timer set register value	*/
//}CMT_CPRIREG_INF;
//
//typedef struct									/* CPRI NE切替情報管理型	*/
//{
//	CMT_RECLS_INF		reclsinf[D_TCM_ACTIVE_LINK_GROUP_NUM];/* RE共通リンク情報			*/
//	CMT_CPRICONF_INF	cpriconf[CMD_CPRIIDX_MAX];	/* CPRIコンフィグ情報	*/
//												/* CPRIリンクステート情報	*/
//	CMT_CPRILNKSTA_INF	cprilinksta[CMD_CPRIIDX_MAX];
//	CMT_CPRIREG_INF		reginf;					/*	レジスタ情報型	*/
//}CMT_CPRINECHGINFMNG_TBL;
//
///****************************************************************************/
///* LAPB N/E切替情報管理テーブル(CMT_XXX_TBL)								*/
///****************************************************************************/
//
//
///* **************************************************************************/
///* MTアドレス設定に関する状態テーブル     									*/
///* **************************************************************************/
//typedef struct
//{
//	USHORT	operating;		/*	操作中										*/
//	USHORT	retry_times;	/*	リトライ回数								*/
//	USHORT	result;			/*	応答結果									*/
//	USHORT	dummy;			/*	ダミー										*/
//}CMT_ADRSETSTA;
//
///* **************************************************************************/
///* 通用操作状態テーブル     												*/
///* **************************************************************************/
//typedef struct
//{
//	USHORT	operating;		/*	操作中								*/
//	USHORT	cpri_no;		/*	操作しているCPRIリンク番号（1～24）	*/
//}CMT_OPSTA;
//
///* **************************************************************************/
///* 強制解放に関する状態テーブル     										*/
///* **************************************************************************/
//typedef struct
//{
//	USHORT	operating;		/*	操作中								*/
//	USHORT	cpri_no;		/*	操作中のCPRIリンク番号（1～24）		*/
//	USHORT	retry_times;	/*	リトライ回数						*/
//}CMT_FRCRLSSTA;
//
///* **************************************************************************/
///* MT操作状態情報テーブル     	*/
///* **************************************************************************/
//typedef struct
//{
//	/*	MTアドレス設定状態（CPRIリンク毎に1本）			*/
//	CMT_ADRSETSTA	mt_addrset_sta[CMD_LINK_NUM_T];
//	CMT_OPSTA		fld_cnt_sta;	/*	FLD-MT接続操作状態	*/
//	CMT_OPSTA		fld_rls_sta;	/*	FLD-MT接続解放操作状態	*/
//	/*	FLD-MT接続強制解放操作状態（0: AMP#1; 1: AMP#2)	*/
//	CMT_FRCRLSSTA	fld_frcrls_sta_amp[CMD_AMPNO_MAX];
//	/*	FLD-MT接続強制解放操作状態（OFTRX毎に１本)	*/
//	CMT_FRCRLSSTA	fld_frcrls_sta_oftrx[CMD_LINK_NUM_T];
//	CMT_OPSTA		fld_cmdsnd_sta;	/*	FLD-MTデータコマンド転送送信状態）	*/
//	CMT_OPSTA		fld_filsnd_sta;	/*	FLD-MTデータファイル転送送信状態）	*/
//	CMT_OPSTA		sv_cnt_sta;		/*	SV-MT接続操作状態					*/
//	CMT_OPSTA		sv_rls_sta;		/*	SV-MT接続解放操作状態				*/
//	/*	SV-MT接続強制解放操作状態（0: AMP#1; 1: AMP#2)	*/
//	CMT_FRCRLSSTA	sv_frcrls_sta_amp[CMD_AMPNO_MAX];
//	/*	SV-MT接続強制解放操作状態（OFTRX毎に１本)	*/
//	CMT_FRCRLSSTA	sv_frcrls_sta_oftrx[CMD_LINK_NUM_T];
//	CMT_OPSTA		sv_snd_sta;		/*	SV-MTデータファイル転送送信状態）	*/
//}CMT_OPSTAINFO;
//
///****************************************************************************/
///* Response	to MK MT client													*/
///****************************************************************************/
//typedef struct
//{
//	UINT	result;							/* 応答結果						*/
//	UINT	cause;							/* 結果詳細						*/
//}CMT_MKRSP;
//
//
///* ======================================================================== */
///* LAPB N/E switch management table											*/
///* ======================================================================== */
//typedef struct
//{
//	UCHAR			status;
//	UCHAR			vr;
//	UCHAR			vs;
//	UCHAR			va;
//	UCHAR			rej_cond;
//	UCHAR			per_bsy;
//	UCHAR			retry_cnt;
//	UCHAR			t1_flg;
//	UCHAR			t2_cnt;
//	UCHAR			t3_flg;
//	UCHAR			t4_flg;
//	UCHAR			dummy;
//}LBT_NESTKTBL;
//
/*! buffer type info table */
typedef struct {
	UINT buftyp;			/*!< buffer type*/
	UINT bufcnt;			/*!< buffer count*/
	UINT busyon_thrshd;		/*!< busy on threshold value*/
	UINT busyoff_thrshd;	/*!< busy off threshold value*/
}CMT_BUFFINF_TBL;

/*! define timeout entry function pointer */
typedef VOID (*T_TCM_TIMEOUT_ENTRY)(bpf_ru_hrtm_key_inf_t * key_inf, UINT timerId);

///*!< TCP link connection notification */
//typedef struct {
//	T_MSGHEAD	head;		/* Message common header	*/
//}CMT_TSKIF_TCPCONNNTC;
//
///*!< TCP link release notification */
//typedef struct {
//	T_MSGHEAD	head;		/* Message common header	*/
//}CMT_TSKIF_TCPRELNTC;
//
///*!< CPRI simulator stop response (CMC -> CPRI-MT) */
//typedef struct
//{
//	T_MSGHEAD				head;				/* Message common header	*/
//	USHORT					linknum;			/* total link number		*/
//	CMT_TSKIF_ONELINKRSP	stopresult[CMD_LINK_NUM_PC];		/* 応答結果					*/
//	USHORT					dummy;				/* ダミー領域				*/
//}CMT_CRDIF_CPRIDBGSTPRSP;
//
///****************************************************************************/
///*!
// * @brief MESSAGE NE data information (CMT_CRDIF_NEDATINF)
// * @note
// *
// */
///****************************************************************************/
//typedef struct
//{
//	CMT_CPRINECHGINFMNG_TBL	cprinechgmnginf;	/* CPRI NE切替情報管理		*/
//												/* LAPB NE切替情報管理		*/
//	LBT_NESTKTBL		lapbnechgmnginf[CMD_CPRIIDX_MAX];
//	UCHAR			recnechgmnginf[1024*172];
//
//}CMT_CRDIF_NEDATINF;
//
///**************************************/
///* スレッド別ログコマンド関数テーブル */
///**************************************/
//typedef struct {
//    CHAR thr_name[CMD_NUM4]; /* スレッド名 */
//	VOID (*sta_fnc)(INT*, FILE*);  /* ステータス関数 */
//	VOID (*res_fnc)(INT*, FILE*);  /* リソース関数 */
//} T_TCM_LOGFNC;
//typedef struct {
//	T_TCM_LOGFNC inf[D_TCM_THRINO_MAX + CMD_NUM1]; /* 情報 */
//} T_TCM_LOGFNC_TBL;
//
//
///**
// * @brief	(0x8A010000) MT Address Setting Request
// * @note	 MT Address Setting Request
// *
// */
//typedef struct{
//	T_MSGHEAD			head;								/**< Message head  */
//	USHORT				link_num;							/**< Cpri Link Num  */
//	USHORT				mt_object;							/**< MT Object  */
//}CMT_TSKIF_MTASTREQ;
//
//
///**
// * @brief	(0x8A010001) MT Address Setting Response
// * @note	MT Address Setting Response
// *
// */
//typedef struct{
//	T_MSGHEAD			head;								/**< Message head  */
//	USHORT				link_num;							/**< Cpri Link Num  */
//	USHORT				mt_object;							/**< MT Object  */
//	USHORT				result;								/**< Response Result  */
//	USHORT				dummy;								/**< Dummy  */
//}CMT_TSKIF_MTASTRSP;
//
//
//
///**************************************/
///* L1ログ							  */
///**************************************/
//typedef struct
//{
//	UCHAR		year;			/*!<   年              */
//	UCHAR		month;			/*!<   月              */
//	UCHAR		day;			/*!<   日              */
//	UCHAR		hour;			/*!<   時              */
//	UCHAR		min;			/*!<   分              */
//	UCHAR		sec;			/*!<   秒              */
//	UCHAR		msec1;			/*!<   ミリ秒          */
//	UCHAR		msec2;
//}T_TCM_BCDTIME;
//
//typedef struct
//{
//	T_TCM_BCDTIME	bcdtime;
//	UINT			linkno;
//	UINT			logpos;
//	UINT			regval[CMD_NUM16];
//	UINT			oam_int_count;
//	UINT			txs_int_count;
//}T_TCM_L1RCVSNDLOG_INFO;
//
//typedef struct
//{
//	UINT					counter_h[CMD_LINK_NUM_T];
//	UINT					counter_l[CMD_LINK_NUM_T];
//	T_TCM_BCDTIME			fsttime;
//	T_TCM_BCDTIME			lsttime;
//	T_TCM_L1RCVSNDLOG_INFO	loginf[CMD_NUM16];
//}T_TCM_L1RCVSNDLOG;

/** IRQ count */
typedef struct{
	UINT	send_IRQ_Count[CMD_LINK_NUM_T];
	UINT	send_Clr_Count[CMD_LINK_NUM_T];
	UINT	recv_IRQ_Count[CMD_LINK_NUM_T];
	UINT	recv_Clr_Count[CMD_LINK_NUM_T];
}T_TCM_IRQ_COUNT;

//#if defined(OPT_CPRI_L2HARD)
///* ======================================================================== */
///* CPRI-MT mode manage table												*/
///* ======================================================================== */
//typedef struct
//{
//	USHORT			simulator;
//	USHORT			monitor;
//	USHORT			cpriTrace;
//	USHORT			dummy;
//}T_TCM_MKMTMODE;
//
///* DMA関連情報(ROMテーブル:cmr_dmaRcvInfo)														*/
//typedef struct
//{
//	UINT					intBit;							/*	割込みbit情報					*/
//	UINT					shiftNum;						/*	割込みbitシフト数				*/
//	UINT					setAdr;							/*	制御アドレス					*/
//}CMT_DMADATINFO;
//#endif
//
//#ifdef D_OPT_CPRING_LOG
///******************************************************************************************************************************/
///** Improve Log Management table
// *  @addtogroup TRIF
// *  @{
// */
///******************************************************************************************************************************/
///**
// * @brief	Buffer Hunt NG Log Management
// * @note
// *
// */
//typedef struct{
//	UINT				sec;								/**< tv_sec of timeval  */
//	UINT				usec;								/**< tv_usec of timeval  */
//	UINT				retVal;								/**< return value  */
//	UINT				bufType;							/**< buffer type  */
//}T_TCM_BUFHNTNGLOGMNG;
//
///**
// * @brief	Buffer Hunt NG Log Management
// * @note
// *
// */
//typedef struct{
//	UINT				sec;								/**< tv_sec of timeval  */
//	UINT				usec;								/**< tv_usec of timeval  */
//	UINT				simMode;							/**< simulator mode status  */
//	UINT				monMode;							/**< monitor mode status  */
//}T_TCM_DMACINTNOCLRLOGMNG;
//
///**
// * @brief	Improve Log Management table
// * @note	Improve Log Managemnt table
// *
// */
//typedef struct{
//	T_TCM_BUFHNTNGLOGMNG	bufHntNgLogMng;					/**< Buffer Hunt NG Log Management  */
//	T_TCM_DMACINTNOCLRLOGMNG	dmacIntNoClrLogMng[CMD_LINK_NUM_T];
//											/**< DMAC Sending Interrupt No Clear Log Management  */
//}T_TCM_IMPRVLOGMNGTBL;
//
///** @} */
//#endif
//
//
/**
 * @brief	AMP接続情報管理テーブル
 * @note
 *
 */
typedef struct{
	USHORT	actlnk_gr;					/**< Active link group(AMP connect information)  */
	USHORT	sys_re_type;				/**< BRE or RRE/LRE or M-LRE                     */
	USHORT	re_serial_idx;				/**< RE serial index                             */
	USHORT	dummy;						/**< dummy                                       */
}T_TCM_AMPCONNMNG;

/**
 * @brief	RE Auto Reset 
 * @note
 *
 */
typedef struct{
	USHORT	ReAutoRstPreventFlg;		/**< RE autonomy reset control flag                   */
	USHORT	ReAutoRstTimFlg;			/**< RE card autonomy reset waiting timer flag        */
	USHORT	ReAutoRstCtlFlg;			/**< Flag when RE autonomy reset is being controlled */
	UINT	resetTime;					/**< RE autonomy reset time                          */
}T_COM_AUTORESETMNGTBL;
//
/** @} */

/**
 * @brief	TXS/RXS割り込み送受信表示ログ 退避用テーブル
 * @note
 *
 */
typedef struct{
	T_TCM_IRQ_COUNT irqcount;
	UINT            cmpcount[CMD_LINK_NUM_T];
	UINT            sndcount[CMD_LINK_NUM_T];
}T_TCM_TXSRXS_COUNT;

/** @} */

/** @} */

/**
 * @brief	レジスタログ 退避用テーブル
 * @note
 *
 */
typedef struct{
	UINT            cpr_err_state[CMD_LINK_NUM_T];	/* CPRIリンク障害要因ステータスレジスタ */
	UINT            cpr_rflm_err[CMD_LINK_NUM_T];	/* CPRIリンク 対向フレーマ障害表示レジスタ */
}T_TCM_REGLOG_TBL;

/** @} */

typedef struct {
	VOID * data;
}T_MSG;


/**
 * @brief   CLIST node
 * @note    this is the most basic data structure for DLIST. \
 *          all data structure, which you want to add to or remove from a linked list, \
 *          is required to inherit this structure.
 *
 */
typedef struct{
    USHORT              nextIndex;                          /**< next node index  */
    USHORT              prevIndex;                          /**< prev node index  */
    USHORT              myIndex;                            /**< my Index  */
    USHORT              listId;                             /**< list identifier  */
}T_TCM_DLIST_NODE;

/**
 * @brief   Double List Management
 * @note
 *
 */
typedef struct{
    UINT                count;                              /**< list counter  */
    USHORT              headIndex;                          /**< head node index  */
    USHORT              tailIndex;                          /**< tail node index  */
    UINT                nodeTbl;                            /**< node table address  */
    USHORT              nodeSize;                           /**< size of node  */
    USHORT              listId;                             /**< list identifier  */
}T_TCM_DLIST_MNG;


/**
 * @brief   timer info manage table
 */
typedef struct{
	USHORT				count;
	USHORT				ofst;
	USHORT				timerID;
	USHORT				times;
	USHORT				timval;		/*	par 10msec	*/
	USHORT				dummy;
	UINT				timvalSav;	/*	par 1msec	*/
}T_TCM_TIMER_INFO;

/**
 * @brief   timer manage table
 */
typedef struct{
	USHORT				numMax;
	USHORT				num;
	USHORT				start;
	USHORT				next;
	USHORT				last;
	USHORT				dummy;
	UINT				tim_info[384];
	UINT				tim_typ[384];
	UCHAR				tim_kind[384];
	UCHAR				tim_cpri[384];
	UCHAR				tim_extNo[384];
	UCHAR				tim_sysNo[384];
	USHORT				tim_before[384];
	USHORT				tim_next[384];
}T_TCM_TIMEOUT_INFO;

typedef struct{
	USHORT				useNumMax;
	USHORT				useNum;
	USHORT				count;
	USHORT				dummy;
	T_TCM_TIMEOUT_INFO	info[256];
}T_TCM_TIMEOUT_MNGTBL;


//#endif /* #if 0 */
#endif
