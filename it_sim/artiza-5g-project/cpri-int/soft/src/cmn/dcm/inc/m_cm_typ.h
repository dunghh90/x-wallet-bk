/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_typ.h
 *  @brief	TRA process Common structure
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2008/01/28 FFCS)Wuh modified for CR-00020-005
 *  @date   2009/03/25	FFCS)Niemsh	M-S3G-eNBPF-01669
 *  @date   2009/06/16  FFCS)Wuh modify for M-S3G-eNBPF-02086, 
 *                      modify for FLASH erase in file no new case faild
 *  @date   2009/07/20  FFCS)Tangj modify for saving the system parameter 
 *                      which is set to hardware
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充
 *  @date   2010/04/02 FJT)Taniguchi CR-XXXXX-XXX:F-971(DCM)対応
 *  @date   2010/05/06 FJT)T.Otani   CR-00091-001:[DCM向け]REハードソフトインタフェース仕様書v2.18版リリース
 *  @date   2010/09/17 CMS)Takahashi M-S3G-eNBPF-03763
 *	@date	2011/02/04 FJT)Tokunaga  M-S3G-eNBSYS-01989
 *  @date   2011/02/25 FJT)Tokunaga  modify for 1.5G-RRE対応
 *  @date   2011/09/23 FFCS)Linlj  modify for 省電力対応
 *  @date   2013/09/30 FFCS)Xut Modify for 13B-03-005-04
 *  @date   2015/04/25 FFCS)Zhangxq TDD SRE
 *  @date   2015/09/10 ALPHA)ueda modify for TDD-RRE(Zynq)
 *  @date   2015/09/11 ALPHA)murakami modify for RRH-006-000
 *  @date   2015/09/14 ALPHA)kamada modify for RRE 3.5G TDD
 *  @date   2015/11/05 ALPHA)ueda modify for TDD-RRE(Zynq)
 *  @date   2015/11/12 ALPHA)ueda modify for TDD-RRE(zynq)
 *  @date	2021/01/21 M&C)Tri.hn Create based on No.29 (seq 3-2) - Add new struct CMT_CPRIF_RERSTRES
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*******************************************************************************/

/* For multiple include  */
#ifndef    M_CM_TYP
#define    M_CM_TYP

/** @addtogroup TRA_COM
 *  @{
  */

/*************************************************************************** */
/*	構造体定義																 */
/*	m_cm_typ.hは以下の構成をとっている										 */
/*		・No.1	標準構文													 */
/*		・No.2	ログフレーム定義											 */
/*		・No.3	CPRI信号インタフェース型定義								 */
/*		・No.4	Common structure for Common function and individual function */
/*		・No.5	タスク間IFフレーム定義										 */
/*		・No.6	カード内共通テーブル構造体定義								 */
/*																			 */
/* (それぞれにラベルがあるので、追記する際はその範囲内への記載を規則とする)	 */
/*************************************************************************** */

#if 0
/* ========================================================================  */
/* No.1	標準構文															 */
/* ========================================================================  */

typedef	unsigned char		UCHAR;
typedef	unsigned short int	USHORT;
typedef	unsigned int		UINT;
typedef	unsigned long		ULONG;
typedef	signed char			CHAR;
typedef	short int			SHORT;
typedef	int					INT;
typedef	long				LONG;
/* typedef	unsigned long long	UWLONG;*/ /* (TBD) */
 typedef	unsigned long 	UWLONG; /* used for compile */
typedef	void				VOID;
typedef	long long int			LLONG;
typedef	unsigned long long int	ULLONG;
/* system共通ヘッダに定義しているためここでは定義しない						*/

#endif

/* ========================================================================  */
/*	No.3	CPRI信号インタフェース型定義									 */
/* ========================================================================  */

/**
 * @brief	health check request
 * @note	MESSAGE		: health check request \n
 * 			Signal kind	: 0x0110 (3G)	0x0111 (S3G) \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ヘルスチェック要求(CMT_CPRIF_HCREQ)							 */
/* 信号種別：0x0110			方向:TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
} __attribute__ ((packed)) CMT_CPRIF_HCREQ ;

/**
 * @brief	health check response
 * @note	MESSAGE		: health check response \n
 * 			Signal kind	: 0x0120 (3G)	0x0121 (S3G) \n
 * 			Direction	: TRA -> TRIF \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ヘルスチェック応答(CMT_CPRIF_HCRES)							 */
/* 信号種別：0x0120		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
/* 2021/03/03 M&C) Update src based on No.27 (seq 9-1,2) - Add start */
	USHORT				result;					/**< 応答結果					 */
/* 2021/03/03 M&C) Update src based on No.27 (seq 9-1,2) - Add end */
} __attribute__ ((packed)) CMT_CPRIF_HCRES ;

/**
 * @brief	RE reset request
 * @note	MESSAGE		: RE reset request \n
 * 			Signal kind	: 0x0210 (3G)	0x0211 (S3G) \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REリセット要求(CMT_CPRIF_RERSTREQ)								 */
/* 信号種別：0x0210		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				kind;					/**< 対象指定					 */
	USHORT				recomapp;				/**< RE共通部指定				 */
} __attribute__ ((packed)) CMT_CPRIF_RERSTREQ;

/**
 * @brief	RE reset response
 * @note	MESSAGE		: RE reset response \n
 * 			Signal kind	: 0x0220 (3G)	0x0221 (S3G) \n
 * 			Direction	: TRA->TRIF \n
 * @date	2021/01/21 M&C)Tri.hn Create based on No.29 (seq 3-2)
 * 
 */

/*************************************************************************** */
/* MESSAGE : REリセット応答(CMT_CPRIF_RERSTRES)								 */
/* 信号種別：0x0220		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
} __attribute__ ((packed)) CMT_CPRIF_RERSTRES;

/**
 * @brief	RE time set indication
 * @note	MESSAGE		: RE time set indication \n
 * 			Signal kind	: 0x0310 (3G)	0x0311 (S3G) \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE時刻設定要求(CMT_CPRIF_RETIMSETREQ)							 */
/* 信号種別：0x0310		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				year;					/**< 年						 */
	USHORT				month_day;				/**< 月日						 */
	USHORT				hour;					/**< (未使用+)時				 */
	USHORT				minute_sec;				/**< 分秒						 */
}__attribute__ ((packed)) CMT_CPRIF_RETIMSETREQ ;

/**
 * @brief	RE common link establish request
 * @note	MESSAGE		: RE common link establish request \n
 * 			Signal kind	: 0x1110 (3G)	0x1111 (S3G) \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE共通部リンク確立要求(CMT_CPRIF_RELINKSTSREQ)					 */
/* 信号種別：0x1110		方向:TRIF->MDIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				link;					/**< リンク設定				 */
}__attribute__ ((packed)) CMT_CPRIF_LINKSTSREQ ;

/**
 * @brief	RE common link establish response
 * @note	MESSAGE		: RE common link establish response \n
 * 			Signal kind	: 0x1120 (3G)	0x1121 (S3G) \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE共通部リンク確立応答(CMT_CPRIF_RELINKSTSRES)					 */
/* 信号種別：0x1120		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
} __attribute__ ((packed)) CMT_CPRIF_LINKSTSRES;

/**
 * @brief	firmware information report request
 * @note	MESSAGE		: firmware information report request \n
 * 			Signal kind	: 0x2110 (3G)	0x2111 (S3G) \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：ファームウェアファイル報告要求(CMT_CPRIF_FIRMFILEREQ)			 */
/* 信号種別：0x2110		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_FIRMFILEREQ ;

/**
 * @brief	firmware information report response
 * @note	MESSAGE		: firmware information report response \n
 * 			Signal kind	: 0x2120 (3G)	0x2121 (S3G) \n
 * 			Direction	: TRA -> TRIF \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファームウェアファイル報告応答(CMT_CPRIF_FIRMFILERES)			 */
/* 信号種別：0x2120		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				makername;				/**< メーカ名					 */
	USHORT				mjr_ver;				/**< メジャーバージョン		 */
	USHORT				mir_ver;				/**< マイナーバージョン		 */
}FIRM_FILE_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	FIRM_FILE_INF		firmfile;			/**< ファームウェアファイル情報群	 */
}__attribute__ ((packed)) CMT_CPRIF_FIRMFILERES ;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				verinf_num;				/*バージョン情報数				 */
	FIRM_FILE_INF		firmfile[CMD_NUM17];	/**< ファームウェアファイル情報群	 */
}__attribute__ ((packed)) CMT_CPRIF_FIRMFILERES2 ;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	file information report notification
 * @note	MESSAGE		: file information report notification \n
 * 			Signal kind	: 0x2210 (3G)	0x2211 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファイル情報報告通知(CMT_CPRIF_FILEINFREQ)						 */
/* 信号種別：0x2210		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				data;					/**< 対象データ				 */
	UCHAR				filename[20];			/**< ファイル名				 */
	UINT				size;					/**< トータルサイズ			 */
}__attribute__ ((packed)) CMT_CPRIF_FILEINFREQ ;

/**
 * @brief	file information report response
 * @note	MESSAGE		: file information report response \n
 * 			Signal kind	: 0x2220 (3G)	0x2221 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファイル情報報告応答(CMT_CPRIF_FILEINFRES)						 */
/* 信号種別：0x2220		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FILEINFRES ;

/**
 * @brief	file data sending
 * @note	MESSAGE		: file data sending \n
 * 			Signal kind	: 0x2300 (3G)	0x2301 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファイルデータ送信(CMT_CPRIF_FILDATSND)						 */
/* 信号種別：0x2300		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				sgmtno1;				/**< 分割番号1				 */
	USHORT				sgmtno2;				/**< 分割番号2				 */
	USHORT				datsize1;				/**< データサイズ1			 */
	USHORT				datsize2;				/**< データサイズ2			 */
	UCHAR				fildat[CMD_FILEDAT_MAX];/**< ファイルデータ			 */
}__attribute__ ((packed)) CMT_CPRIF_FILEDATSND ;

/**
 * @brief	file data sending
 * @note	MESSAGE		: file data sending \n
 * 			Signal kind	: 0x2300 (3G)	0x2301 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファイルデータ送信(CMT_CPRIF_FILDATSND)						 */
/* 信号種別：0x2300		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別				 */
	USHORT				sgmtno1;				/**< 分割番号1				 */
	USHORT				sgmtno2;				/**< 分割番号2				 */
	USHORT				datsize1;				/**< データサイズ1			 */
	USHORT				datsize2;				/**< データサイズ2			 */
	UINT 				offset;					/**< offset					 */
}__attribute__ ((packed)) CMT_CPRIF_FILEDATSNDINT ;

/**
 * @brief	file data sending complete report notice
 * @note	MESSAGE		: file data sending complete report notice \n
 * 			Signal kind	: 0x2310 (3G)	0x2311 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファイルデータ送信完了報告通知(CMT_CPRIF_FILSNDFINNTC)		 */
/* 信号種別：0x2310		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				tgtdat;					/**< 対象データ				 */
	USHORT				chksum;					/**< チェックサム				 */
}__attribute__ ((packed)) CMT_CPRIF_FILSNDFINNTC ;

/**
 * @brief	file data sending complete report response
 * @note	MESSAGE		: file data sending complete report response \n
 * 			Signal kind	: 0x2320 (3G)	0x2321 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：ファイルデータ送信完了報告応答(CMT_CPRIF_FILSNDFINRES)		 */
/* 信号種別：0x2320		方向:TRA->TRIF								 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FILSNDFINRES ;

/*************************************************************************** */
/* MESSAGE ：ファイルデータ送信完了報告応答for S3G(CMT_CPRIF_FILSNDFINRES)		 */
/* 信号種別：0x2321		方向:TRA->TRIF								 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
    USHORT              primary_sys;            /**< primary system */
}__attribute__ ((packed)) CMT_CPRIF_FILSNDFINRES_S3G ;

/**
 * @brief	firmware information report request in use state
 * @note	MESSAGE		: firmware information report request in use state \n
 * 			Signal kind	: 0x2411 \n
 * 			Direction	: TRIF -> TRA \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：運用中ファームウェアファイル報告要求(CMT_CPRIF_UFIRMFILEREQ)			 */
/* 信号種別：0x2411		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_UFIRMFILEREQ ;

/**
 * @brief	firmware information report response in use state
 * @note	MESSAGE		: firmware information report response in use state \n
 * 			Signal kind	: 0x2421 \n
 * 			Direction	: TRA -> TRIF \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：運用中ファームウェアファイル報告応答(CMT_CPRIF_UFIRMFILERES)			 */
/* 信号種別：0x2421		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				makername;				/**< メーカ名					 */
	USHORT				cur_mjr_ver;				/**< 運用中メジャーバージョン		 */
	USHORT				cur_mir_ver;				/**< 運用中マイナーバージョン		 */
	USHORT				nes_mjr_ver;				/**< 起動用メジャーバージョン		 */
	USHORT				nes_mir_ver;				/**< 起動用マイナーバージョン		 */
}UFIRM_FILE_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	UFIRM_FILE_INF		firmfile;			/**< ファームウェアファイル情報群	 */
}__attribute__ ((packed)) CMT_CPRIF_UFIRMFILERES ;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				verinf_num;				/*バージョン情報数				 */
	UFIRM_FILE_INF		firmfile[CMD_NUM17];	/**< ファームウェアファイル情報群*/
}__attribute__ ((packed)) CMT_CPRIF_UFIRMFILERES2 ;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	file information report notification in use state
 * @note	MESSAGE		: file information report notification in use state \n
 * 			Signal kind	: 0x2511 \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：運用中ファイル情報報告通知(CMT_CPRIF_UFILEINFREQ)						 */
/* 信号種別：0x2511		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				data;					/**< 対象データ				 */
	UCHAR				filename[20];			/**< ファイル名				 */
	UINT				size;					/**< トータルサイズ			 */
}__attribute__ ((packed)) CMT_CPRIF_UFILEINFREQ ;

/**
 * @brief	file information report response in use state
 * @note	MESSAGE		: file information report response in use state \n
 * 			Signal kind	: 0x2521 \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：運用中ファイル情報報告応答(CMT_CPRIF_UFILEINFRES)						 */
/* 信号種別：0x2521		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_UFILEINFRES ;

/**
 * @brief	file data sending in use state
 * @note	MESSAGE		: file data sending in use state \n
 * 			Signal kind	: 0x2601 \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：運用中ファイルデータ送信(CMT_CPRIF_UFILEDATSND)						 */
/* 信号種別：0x2601		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				sgmtno1;				/**< 分割番号1				 */
	USHORT				sgmtno2;				/**< 分割番号2				 */
	USHORT				datsize1;				/**< データサイズ1			 */
	USHORT				datsize2;				/**< データサイズ2			 */
	UCHAR				fildat[CMD_FILEDAT_MAX];/**< ファイルデータ			 */
}__attribute__ ((packed)) CMT_CPRIF_UFILEDATSND ;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*************************************************************************** */
/* MESSAGE ：運用中ファイルデータ送信(CMT_CPRIF_UFILEDATSND)						 */
/* 信号種別：0x2601		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				sgmtno1;				/**< 分割番号1				 */
	USHORT				sgmtno2;				/**< 分割番号2				 */
	USHORT				datsize1;				/**< データサイズ1			 */
	USHORT				datsize2;				/**< データサイズ2			 */
	UINT				offset					;/**< offset				 */
}__attribute__ ((packed)) CMT_CPRIF_UFILEDATSNDINT ;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	file data sending complete report notice in use state
 * @note	MESSAGE		: file data sending complete report notice in use state \n
 * 			Signal kind	: 0x2611 \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：運用中ファイルデータ送信完了報告通知(CMT_CPRIF_UFILSNDFINNTC)		 */
/* 信号種別：0x2611		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				tgtdat;					/**< 対象データ				 */
	USHORT				chksum;					/**< チェックサム				 */
}__attribute__ ((packed)) CMT_CPRIF_UFILSNDFINNTC ;

/**
 * @brief	file data sending complete report response in use state
 * @note	MESSAGE		: file data sending complete report response in use state \n
 * 			Signal kind	: 0x2621 \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：運用中ファイルデータ送信完了報告応答(CMT_CPRIF_UFILSNDFINRES)		 */
/* 信号種別：0x2621		方向:TRA->TRIF								 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_UFILSNDFINRES ;

/**
 * @brief	TRX set request
 * @note	MESSAGE		: TRX set request \n
 * 			Signal kind	: 0x4110 (3G)	0x4111 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：TRX設定要求(CMT_CPRIF_TRXSETREQ) -3G								 */
/* 信号種別：0x4110		方向：3G BTS TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				carno;					/**< キャリア番号				 */
	USHORT				frqno;					/**< 周波数番号				 */
}__attribute__ ((packed)) CMT_CPRIF_TRXSETREQ ;

/*************************************************************************** */
/* MESSAGE ：TRX設定要求(CMT_CPRIF_TRXSETREQ) -S3G								 */
/* 信号種別：0x4111		方向：S3G TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				carno;					/**< キャリア番号				 */
	USHORT				bandwidth;					/**< システム帯域幅				 */
	USHORT				dl_freq;				/**< 下り周波数番号  */
	USHORT				snd_br;					/**< 送信系 */
	USHORT				tdd_up_dwn_config;		/**< 下り周波数番号が3.5GHz帯の場合のみ有効とする．     それ以外の下り周波数番号の場合は、Don’t careとする． */
	USHORT				tdd_spe_sf_config;		/**< 下り周波数番号が3.5GHz帯の場合のみ有効とする．  それ以外の下り周波数番号の場合は、Don’t careとする． */
	USHORT				tdd_dwnlk_cp_len;
								/*     0：Normal cyclic prefix								*/
								/*     1：Extended cyclic prefix							*/
								/*     下り周波数番号が3.5GHz帯の場合のみ有効とする．		*/
								/*     それ以外の下り周波数番号の場合は、Don’t careとする	*/
}__attribute__ ((packed)) CMT_CPRIF_TRXSETREQ_S3G ;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/****************************************************************************/
/* MESSAGE ：TRX設定要求(CMT_CPRIF_TRXSETREQ) -S3G(FDD)						*/
/* 信号種別：0x4111		方向：S3G TRIF->TRA									*/
/****************************************************************************/
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別				*/
	USHORT				carno;					/**< キャリア番号			*/
	USHORT				bandwidth;				/**< システム帯域幅			*/
	USHORT				dl_freq;				/**< 下り周波数番号  		*/
	USHORT				snd_br;					/**< 送信系 				*/
}__attribute__ ((packed)) CMT_CPRIF_TRXSETREQ_S3G_FDD;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	TRX set response
 * @note	MESSAGE		: TRX set response \n
 * 			Signal kind	: 0x4120 (3G)	0x4121 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
 
/*************************************************************************** */
/* MESSAGE ：TRX設定応答(CMT_CPRIF_TRXSETRES) 								 */
/* 信号種別：0x4120		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				carno;					/**< キャリア番号				 */
}__attribute__ ((packed)) CMT_CPRIF_TRXSETRES ;

/**
 * @brief	TRX release request
 * @note	MESSAGE		: TRX release request \n
 * 			Signal kind	: 0x4210 (3G)	0x4211 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：TRX解放要求(CMT_CPRIF_TRXRELREQ) 								 */
/* 信号種別：0x4210		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				carno;					/**< キャリア番号				 */
} __attribute__ ((packed)) CMT_CPRIF_TRXRELREQ;

/**
 * @brief	TRX release response
 * @note	MESSAGE		: TRX release response \n
 * 			Signal kind	: 0x4220 (3G)	0x4221 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
/*************************************************************************** */
/* MESSAGE ：TRX解放応答(CMT_CPRIF_TRXRELRES) 								 */
/* 信号種別：0x4220		方向：MDIF->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				carno;					/**< キャリア番号				 */
}__attribute__ ((packed)) CMT_CPRIF_TRXRELRES ;

/**
 * @brief	Toffset report request
 * @note	MESSAGE		: Toffset report request \n
 * 			Signal kind	: 0x4310 (3G)	0x4311 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：Toffset値報告要求(CMT_CPRIF_TOFFSETINFREQ)						 */
/* 信号種別：0x4310		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_TOFFSETINFREQ ;

/**
 * @brief	Toffset report response
 * @note	MESSAGE		: Toffset report response \n
 * 			Signal kind	: 0x4320 (3G)	0x4321 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：Toffset値報告応答(CMT_CPRIF_TOFFSETINFRES)						 */
/* 信号種別：0x4320		方向:TRA->TRIF										 */
/*************************************************************************** */
/* Toffset値情報  */
typedef struct
{
	USHORT				toffset;				/**< Toffset値				 */
	USHORT				updeleqp;				/**< 上り装置内遅延値			 */
	USHORT				downdeleqp;				/**< 下り装置内遅延値			 */
}TOFFSET_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	TOFFSET_INF			toffset_inf;			/**< Toffset値情報			 */
}__attribute__ ((packed)) CMT_CPRIF_TOFFSETINFRES ;

/**
 * @brief	no modulation carrier ON/OFF specify request
 * @note	MESSAGE		: no modulation carrier ON/OFF specify request \n
 * 			Signal kind	: 0x4410 (3G)	0x4411 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：無変調キャリア出力ON/OFF指定要求(CMT_CPRIF_TRXRELREQ)-3G 			 */
/* 信号種別：0x4410		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				carno;					/**< キャリア番号				 */
	USHORT				br_app;					/**< 系指定					 */
	USHORT				onoff_app;				/**< ON/OFF指定				 */
	USHORT				sndpwr;					/**< 送信電力					 */
}__attribute__ ((packed)) CMT_CPRIF_CARONOFFREQ ;

/*************************************************************************** */
/* MESSAGE ：無変調キャリア出力ON/OFF指定要求(CMT_CPRIF_TRXRELREQ)-S3G 			 */
/* 信号種別：0x4411		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				carno;					/**< キャリア番号				 */
	USHORT				onoff_app;				/**< ON/OFF指定				 */
	USHORT				sndpwr;					/**< 送信電力					 */
}__attribute__ ((packed)) CMT_CPRIF_CARONOFFREQ_S3G ;

/**
 * @brief	no modulation carrier ON/OFF specify response
 * @note	MESSAGE		: no modulation carrier ON/OFF specify response \n
 * 			Signal kind	: 0x4420 (3G)	0x4421 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：無変調キャリア出力ON/OFF指定応答(CMT_CPRIF_CARONOFFRES)		 */
/* 信号種別：0x4420		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				carno;					/**< キャリア番号				 */
	USHORT				br_app;					/**< 系指定					 */
}__attribute__ ((packed)) CMT_CPRIF_CARONOFFRES ;

/*************************************************************************** */
/* MESSAGE ：無変調キャリア出力ON/OFF指定応答(CMT_CPRIF_CARONOFFRES_S3G)		 */
/* 信号種別：0x4421		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				carno;					/**< キャリア番号				 */
}__attribute__ ((packed)) CMT_CPRIF_CARONOFFRES_S3G ;

/**
 * @brief	RE equipment composition information report request
 * @note	MESSAGE		: RE equipment composition information report request \n
 * 			Signal kind	: 0x5110 (3G)	0x5111 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：RE装置構成情報報告要求(CMT_CPRIF_EQPSTRREQ)					 */
/* 信号種別：0x5110		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_EQPSTRREQ ;


/**
 * @brief	RE equipment composition information report response
 * @note	MESSAGE		: RE equipment composition information report response \n
 * 			Signal kind	: 0x5120 (3G)	0x5121 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE装置構成情報報告応答(CMT_CPRIF_EQPSTRREP)		for 3G			 */
/* 信号種別：0x5120		方向:TRA->TRIF										 */
/*************************************************************************** */
/* CPRIリンク装置構成情報  */
typedef struct
{
	USHORT				maker;					/**< メーカ名					 */
	USHORT				bts_no;					/**< BTS形号					 */
	USHORT				rekind;					/**< RE種別					 */
	USHORT				eqp_no;					/**< 装置号番					 */
	USHORT				dummy1[6];				/**< 空き						 */
	USHORT				powerkind;				/**< 電源種別					 */
	USHORT				powerunit;				/**< パワークラス単位			 */
	USHORT				powerclass_0;			/**< パワークラス(0系)		 */
	USHORT				powerclass_1;			/**< パワークラス(1系)		 */
	USHORT				frqband;				/**< 対応周波数帯				 */
	USHORT				us800m_fil_ytp;			/**< 800MHz帯フィルタタイプ★	 */
	USHORT				us2g_car_no;			/**< 2GHz帯キャリア数		★	 */
	USHORT				us800m_car_no;			/**< 800MHz帯キャリア数	★	 */
	USHORT				us17g_car_no;			/**< 1.7GMHz帯キャリア数	★	 */
	USHORT				trx_inf_type;			/**< RE側TRX-INFタイプ情報	 */
	USHORT				us15g_car_no;			/**< 1.5GMHz帯キャリア数	★	 */
	USHORT				dummy2[43];				/**< 空き						 */
}CPRIEQPSTR_INF;

/*************************************************************************** */
/* MESSAGE ：RE装置構成情報報告応答(CMT_CPRIF_EQPSTRREP)	for S3G			 */
/* 信号種別：0x5121		方向:TRA->TRIF										 */
/*************************************************************************** */
/* CPRIリンク装置構成情報  */
typedef struct
{
	USHORT				maker;					/**< メーカ名					 */
	USHORT				bts_no;					/**< BTS形号					 */
	USHORT				eqp_no;					/**< 装置号番					 */	
	USHORT				rekind;					/**< RE種別					 */
	USHORT				bts_share_type;			/**< 3G BTS share type	 */
	USHORT				dummy1;					/**< 空き						 */
	USHORT				powerkind;				/**< 電源種別					 */
	USHORT				rcv_antena_no;				/**< receiving antena number		*/		
	USHORT				snd_antena_no;				/**< sending antena number		*/	
	USHORT				powerunit;				/**< パワークラス単位			 */	
	USHORT				powerclass_0;			/**< パワークラス(0系)		 */
	USHORT				powerclass_1;			/**< パワークラス(1系)		 */
	USHORT				powerclass_2;			/**< パワークラス(2系)		 */
	USHORT				powerclass_3;			/**< パワークラス(3系)		 */	
	USHORT				dummy2[2];				/**< 空き						 */	
	USHORT				frqband;				/**< 対応周波数帯				 */
	USHORT				max_bandwidth;			/**< maximum bandwidth	 */
	USHORT				us2g_car_no;			/**< 2GHz帯キャリア数		★	 */
	USHORT				us15g_car_no;			/**< 1.5GHz帯キャリア数			 */
	USHORT				us800m_car_no;			/**< 800MHz帯キャリア数		★	 */
	USHORT				us700m_car_no;			/**< 700MHz帯キャリア数			 */
	USHORT				us17g_car_no;			/**< 1.7GHz帯キャリア数			 */
	USHORT				dummy3[1];				/**< 空き						 */
	USHORT				trx_inf_type;			/**< RE側TRX-INFタイプ情報	 */
	USHORT				us35g_car_no;			/**< キャリア数(3.5GHz帯)		 */
	USHORT				powerclass_4;			/**< パワークラス(4系)			 */
	USHORT				powerclass_5;			/**< パワークラス(5系)			 */
	USHORT				powerclass_6;			/**< パワークラス(6系)			 */
	USHORT				powerclass_7;			/**< パワークラス(7系)			 */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT				us34g_car_no;			/**< キャリア数(3.4GHz帯)		 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT				dummy4[34];				/**< 空き						 */
}__attribute__ ((packed)) CPRIEQPSTR_INF_S3G ;


typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	CPRIEQPSTR_INF		cpristr_inf;			/**< CPRIリンク装置構成情報	 */
}__attribute__ ((packed)) CMT_CPRIF_EQPSTRRES ;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	CPRIEQPSTR_INF_S3G		cpristr_inf;			/**< CPRIリンク装置構成情報	 */
}__attribute__ ((packed)) CMT_CPRIF_EQPSTRRES_S3G ;

/**
 * @brief	RE slot information report request
 * @note	MESSAGE		: RE slot information report request \n
 * 			Signal kind	: 0x5210 (3G)	0x5211 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REスロット情報報告要求(CMT_CPRIF_SLOTINFREQ)					 */
/* 信号種別：0x5210		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_SLOTINFREQ ;

/**
 * @brief	RE slot information report response
 * @note	MESSAGE		: RE slot information report response \n
 * 			Signal kind	: 0x5220 (3G)	0x5221 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：REスロット情報報告応答(CMT_CPRIF_SLOTINFRES)					 */
/* 信号種別：0x5220		方向:TRA->TRIF										 */
/*************************************************************************** */
/* SLOT単位の詳細情報  */
typedef struct
{
	USHORT				slot_no;				/**< SLOT番号					 */
	USHORT				card_inf;				/**< REカード機能部情報		 */
}SLOT_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_SLOTINFRES ;

/**
 * @brief	RE status report request
 * @note	MESSAGE		: RE status report request \n
 * 			Signal kind	: 0x5310 (3G)	0x5311 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE状態報告要求(CMT_CPRIF_RESTSREQ)								 */
/* 信号種別：0x5310		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_RESTSREQ ;

/**
 * @brief	RE status report response
 * @note	MESSAGE		: RE status report response \n
 * 			Signal kind	: 0x5320 (3G)	0x5321 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE状態報告応答(CMT_CPRIF_RESTSRES)								 */
/* 信号種別：0x5320		方向:TRA->TRIF										 */
/*************************************************************************** */
/* RE状態報告詳細  */
typedef struct
{
	USHORT				maker_name;				/**< メーカ名					 */
/* =========================== 可変長情報 =================================  */
/* ※メーカ名以降は以下の可変長情報が付加される。							 */
/* ■パラメータ		■サイズ	■値		■説明							 */
/* ・FAN情報数N		2Byte		0～			FAN ALM情報の数					 */
/* ・FAN情報#1		2Byte		1～			FAN ALM箇所を通知する情報		 */
/* 	～	(FAN情報数N個分FAN情報有効)											 */
/* ・FAN情報#n		2Byte		1～			FAN ALM箇所を通知する情報		 */
/*																			 */
/* ・メーカ固有RE															 */
/*			情報数N	2Byte		0～			メーカ固有のREに関するALM情報数	 */
/* ・メーカ固有RE															 */
/*			情報#1	2Byte		1～			メーカ固有のREに関するALM情報	 */
/* 	～	(メーカ固有RE情報数N個分メーカ固有RE情報有効)						 */
/* ・メーカ固有RE															 */
/*			情報#n	2Byte		1～			メーカ固有のREに関するALM情報	 */
/* ========================================================================  */
}RESTS_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	RESTS_INF			rests_inf;				/**< RE状態報告詳細			 */
}__attribute__ ((packed)) CMT_CPRIF_RESTSRES ;

/**
 * @brief	RE card information report request
 * @note	MESSAGE		: RE card information report request \n
 * 			Signal kind	: 0x5410 (3G)	0x5411 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REカード情報報告要求(CMT_CPRIF_CARDINFREQ)						 */
/* 信号種別：0x5410		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				slot_no;				/**< スロット番号				 */
}__attribute__ ((packed)) CMT_CPRIF_CARDINFREQ ;

/**
 * @brief	RE card information report response
 * @note	MESSAGE		: RE card information report response \n
 * 			Signal kind	: 0x5420 (3G)	0x5421 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REカード情報報告応答(CMT_CPRIF_CARDINFRES)						 */
/* 信号種別：0x5420		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				slot_no;				/**< スロット番号				 */
	UCHAR				meker_name[16];			/**< メーカ名					 */
	UCHAR				card_name[8];			/**< カード名					 */
	USHORT				up_resrc_num1;			/**< 上りリソース数1			 */
	USHORT				up_resrc_num2;			/**< 上りリソース数2			 */
	USHORT				down_resrc_num1;		/**< 下りリソース数1			 */
	USHORT				down_resrc_num2;		/**< 下りリソース数2			 */
	USHORT				card_svitem;			/**< カード基本監視項目		 */
	USHORT				card_extitem;			/**< カード拡張監視項目		 */
	USHORT				card_cntitem;			/**< カード制御項目			 */
	USHORT				funcitem;				/**< 機能部固有制御項目		 */
}__attribute__ ((packed)) CMT_CPRIF_CARDINFRES ;

/**
 * @brief	RE version report request
 * @note	MESSAGE		: RE version report request \n
 * 			Signal kind	: 0x5510 (3G)	0x5511 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：REバージョン報告要求(CMT_CPRIF_REVERREQ) 						 */
/* 信号種別：0x5510		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				tgtkind;				/**< 対象指定					 */
	USHORT				slot_no;				/**< スロット番号				 */
	USHORT				id_kind;				/**< ID情報識別				 */
}__attribute__ ((packed)) CMT_CPRIF_REVERREQ ;

/**
 * @brief	RE version report response
 * @note	MESSAGE		: RE version report response \n
 * 			Signal kind	: 0x5520 (3G)	0x5521 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：REバージョン報告応答(CMT_CPRIF_REVERRES) 						 */
/* 信号種別：0x5520		方向:TRA->TRIF										 */
/*************************************************************************** */
/* REバージョン情報詳細  */
typedef struct
{
	UCHAR				name[10];				/**< 名称						 */
	USHORT				mjr_ver;				/**< メジャーバージョン		 */
	USHORT				mir_ver;				/**< マイナーバージョン		 */
	USHORT				year;					/**< 年						 */
	USHORT				month_day;				/**< 月日						 */
}REVER_INF;

/* REバージョン報告詳細  */
typedef struct
{
	UCHAR				maker_name[16];			/**< メーカ名					 */
	USHORT				kind;					/**< 対象指定					 */
	USHORT				slot_no;				/**< スロット番号				 */
	UCHAR				id_kind[1024];			/**< ID情報識別				 */
	USHORT				inf_num;				/**< 情報数					 */
}RESTA_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	RESTA_INF			rests_inf;				/**< RE状態報告詳細			 */
}__attribute__ ((packed)) CMT_CPRIF_REVERRES ;

/**
 * @brief	RE card control request
 * @note	MESSAGE		: RE card control request \n
 * 			Signal kind	: 0x5610 (3G)	0x5611 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：REカード制御要求(CMT_CPRIF_RECARDCNTREQ)						 */
/* 信号種別：0x5610		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				slot_no;				/**< スロット番号				 */
	USHORT				cnt_kind;				/**< 制御						 */
	USHORT				cnt_code;				/**< 制御コード				 */
}__attribute__ ((packed)) CMT_CPRIF_RECARDCNTREQ ;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
typedef struct
{
    USHORT              signal_kind;            /**< 信号種別                   */
    USHORT              slot_no;                /**< スロット番号               */
    USHORT              cnt_kind;               /**< 制御                       */
}__attribute__ ((packed)) CMT_CPRIF_RECARDCNTREQ_S3G;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	RE card control response
 * @note	MESSAGE		: RE card control response \n
 * 			Signal kind	: 0x5620 (3G)	0x5621 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：REカード制御応答(CMT_CPRIF_RECARDCNTRES)						 */
/* 信号種別：0x5620		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				slot_no;				/**< スロット番号				 */
}__attribute__ ((packed)) CMT_CPRIF_RECARDCNTRES ;

/**
 * @brief	RE card status report request
 * @note	MESSAGE		: RE card status report request \n
 * 			Signal kind	: 0x5710 (3G)	0x5711 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：REカード状態報告要求(CMT_CPRIF_CARDSTSREQ)						 */
/* 信号種別：0x5710		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				slot_no;				/**< スロット番号				 */
}__attribute__ ((packed)) CMT_CPRIF_CARDSTSREQ ;

/**
 * @brief	RE card status report response
 * @note	MESSAGE		: RE card status report response \n
 * 			Signal kind	: 0x5720 (3G)	0x5721 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REカード状態報告応答(CMT_CPRIF_CARDSTSRES)						 */
/* 信号種別：0x5720		方向:TRA->TRIF										 */
/*************************************************************************** */
/* カード情報  */
typedef struct
{
	USHORT				slot_no;				/**< スロット番号				 */
	USHORT				year;					/**< 年						 */
	USHORT				month_day;				/**< 月日						 */
	USHORT				hour;					/**< 時						 */
	USHORT				minsec;					/**< 分秒						 */
	USHORT				card_svitem;			/**< カード基本監視項目		 */
	USHORT				card_extitem;			/**< カード拡張監視項目		 */
	USHORT				card_cntitem;			/**< カード制御項目			 */
	USHORT				cnt_code;				/**< 制御コード				 */
	USHORT				funcitem;				/**< 機能部固有制御項目		 */
	USHORT				car_inf;				/**< キャリア情報				 */
/* =========================== 可変長情報 =================================  */
/* ※キャリア情報以降は以下の可変長情報が付加される。						 */
/* ■パラメータ		■サイズ	■値		■説明							 */
/* ・ALM情報数N		2Byte		0～255		発生しているALMの種類数			 */
/* ・ALM情報#1		2Byte		1～255		各ALMコード						 */
/* 	～	(ALM情報数N個分ALM情報有効)											 */
/* ・ALM情報#n		2Byte		1～255		各ALMコード						 */
/*																			 */
/* ・ERR情報数N		2Byte		0～255		発生しているERRの種類数			 */
/* ・ERR情報#1		2Byte		1～255		各ERRコード						 */
/* 	～	(ERR情報数N個分ERR情報有効)											 */
/* ・ERR情報#n		2Byte		1～255		各ERRコード						 */
/* ========================================================================  */
}CARDSTS_INF;


/**
 * @brief	RE card status report response
 * @note	MESSAGE		: RE card status report response \n
 * 			Signal kind	: 0x5721 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REカード状態報告応答(CMT_CPRIF_CARDSTSRES_S3G)						 */
/* 信号種別：0x5721		方向:TRA->TRIF			for S3G							 */
/*************************************************************************** */
/* カード情報  */
typedef struct
{
	USHORT				slot_no;				/**< スロット番号				 */
	UCHAR				card_name[CMD_NUM8];	/**< Card name  */
	USHORT				year;					/**< 年						 */
	USHORT				month_day;				/**< 月日						 */
	USHORT				hour;					/**< 時						 */
	USHORT				minsec;					/**< 分秒						 */
	USHORT				card_svitem;			/**< カード基本監視項目		 */
	USHORT				card_extitem;			/**< カード拡張監視項目		 */
	USHORT				card_cntitem;			/**< カード基本制御項目			 */
	USHORT				card_extcntitem;		/**< カード拡張制御項目			 */	
/* =========================== 可変長情報 =================================  */
/* ※キャリア情報以降は以下の可変長情報が付加される。						 */
/* ■パラメータ		■サイズ	■値		■説明							 */
/* ・機能部数			2Byte		0～28				 */
/* ・機能部#1機能部種別		2Byte	0～28				 */
/* ・機能部#1機能部故障状態		2Byte	0-3					 */
/* ・機能部#1 機能部状態基本監視項目 2byte TBD			 */
/* ・機能部#1 機能部状態拡張監視項目 2byte TBD			 */
/* ・機能部#1 対象セクタ数（T.B.D.）2byte		 */
/* ・機能部#1 対象セクタ.キャリア#1 セクタ番号 2byte */
/* ・機能部#1 対象セクタ.キャリア#1 キャリア番号 2byte  */
/* ・機能部#1 対象セクタ.キャリア#1 機能部状態 2byte  */
/* ・機能部#1 対象セクタ.キャリア#2 セクタ番号 2byte  */
/* ・機能部#1 対象セクタ.キャリア#2 キャリア番号 2byte  */
/* ・機能部#1 対象セクタ.キャリア#2 機能部状態 2byte  */
/*																			 */
/* ・機能部#n機能部種別		2Byte	0～28				 */
/* ・機能部#n機能部故障状態		2Byte	0-3					 */
/* ・機能部#n 機能部状態基本監視項目 2byte TBD			 */
/* ・機能部#n 機能部状態拡張監視項目 2byte TBD			 */
/* ・機能部#n 対象セクタ数（T.B.D.）2byte		 */
/* ・機能部#n 対象セクタ.キャリア#1 セクタ番号 2byte */
/* ・機能部#n 対象セクタ.キャリア#1 キャリア番号 2byte  */
/* ・機能部#n 対象セクタ.キャリア#1 機能部状態 2byte  */
/* ・機能部#n 対象セクタ.キャリア#2 セクタ番号 2byte  */
/* ・機能部#n 対象セクタ.キャリア#2 キャリア番号 2byte  */
/* ・機能部#n 対象セクタ.キャリア#2 機能部状態 2byte  */

/* ・ALM情報数N		2Byte		0～255		発生しているALMの種類数			 */
/* ・ALM情報#1		2Byte		1～255		各ALMコード						 */
/* 	～	(ALM情報数N個分ALM情報有効)											 */
/* ・ALM情報#n		2Byte		1～255		各ALMコード						 */
/*																			 */
/* ・ERR情報数N		2Byte		0～255		発生しているERRの種類数			 */
/* ・ERR情報#1		2Byte		1～255		各ERRコード						 */
/* 	～	(ERR情報数N個分ERR情報有効)											 */
/* ・ERR情報#n		2Byte		1～255		各ERRコード						 */
/* ========================================================================  */
}CARDSTS_INF_S3G;


typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	CARDSTS_INF			cardsts_inf;			/**< カード状態詳細			 */
}__attribute__ ((packed)) CMT_CPRIF_CARDSTSRES ;


typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	CARDSTS_INF_S3G			cardsts_inf;			/**< カード状態詳細			 */
}__attribute__ ((packed)) CMT_CPRIF_CARDSTSRES_S3G ;


/**
 * @brief	RE carrier status report request
 * @note	MESSAGE		: RE carrier status report request \n
 * 			Signal kind	: 0x5810 (3G)	0x5811 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：REキャリア状態報告要求(CMT_CPRIF_CARLSTSREQ)					 */
/* 信号種別：0x5810		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_CARLSTSREQ ;

/**
 * @brief	RE carrier status report response
 * @note	MESSAGE		: RE carrier status report response \n
 * 			Signal kind	: 0x5820 (3G)	0x5821 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REキャリア状態報告応答(CMT_CPRIF_CARLSTSRES)					 */
/* 信号種別：0x5820		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				car_num;				/**< キャリア数				 */
//	USHORT				carstsinf;				/**< キャリア状態詳細先頭情報	 */
/* =========================== キャリア状態詳細 ===========================  */
/* ※TRX設定済みのキャリア数分、以下の情報がキャリア状態詳細先頭情報		 */
/*	 (1つ目のキャリア番号に該当)から付加される								 */
/* ■パラメータ		■サイズ	■値		■説明							 */
/* ・キャリア番号	2Byte		1～4		TRX設定済みのキャリア番号		 */
/* ・キャリア状態	2Byte		-			TRX設定済みキャリアの状態		 */
/* ・キャリア構成状態	2Byte	0～2		送信可否情報					 */
/*																			 */
/* ※キャリア構成状態以降は以下の可変長情報が付加される。					 */
/* ■パラメータ		■サイズ	■値		■説明							 */
/* ・ALM情報数N		2Byte		0～255		発生しているALMの種類数			 */
/* ・ALM情報#1		2Byte		221～255	各ALMコード						 */
/* 	～	(ALM情報数N個分ALM情報有効)											 */
/* ・ALM情報#n		2Byte		221～255	各ALMコード						 */
/*																			 */
/* ・ERR情報数N		2Byte		0～255		発生しているERR種類数			 */
/* ・ERR情報#1		2Byte		221～255	各ERRコード						 */
/* 	～	(ERR情報数N個分ERR情報有効)											 */
/* ・ERR情報#n		2Byte		221～255	各ERRコード						 */
/* 																			 */
/* ・カード数		2Byte		0～8		該当キャリアが割り当てられている */
/* 											カード数						 */
/* ・スロット番号#1	2Byte		1～56		REのスロット番号				 */
/* 	～	（カード数分スロット番号有効）										 */
/* ・スロット番号#n	2Byte		1～56		REのスロット番号				 */
/* ========================================================================  */
}__attribute__ ((packed)) CMT_CPRIF_CARLSTSRES;

/**
 * @brief	RE port control request
 * @note	MESSAGE		: RE port control request \n
 * 			Signal kind	: 0x5910 (3G)	0x5911 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE PORT制御要求(CMT_CPRIF_REPORTCNTREQ)						 */
/* 信号種別：0x5910		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				infbit;					/**< 情報ビット				 */
	USHORT				control;				/**< 制御						 */
}__attribute__ ((packed)) CMT_CPRIF_REPORTCNTREQ;

/**
 * @brief	RE port control response
 * @note	MESSAGE		: RE port control response \n
 * 			Signal kind	: 0x5920 (3G)	0x5921 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：RE PORT制御応答(CMT_CPRIF_REPORTCNTRES)						 */
/* 信号種別：0x5920		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				portout_sts;			/**< PORT出力状態				 */
}__attribute__ ((packed)) CMT_CPRIF_REPORTCNTRES ;

/**
 * @brief	RE port status report request
 * @note	MESSAGE		: RE port status report request \n
 * 			Signal kind	: 0x5A10 (3G)	0x5A11 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE PORT状態報告要求(CMT_CPRIF_REPORTSTSREQ)					 */
/* 信号種別：0x5A10		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_REPORTSTSREQ ;

/**
 * @brief	RE port status report response
 * @note	MESSAGE		: RE port status report response \n
 * 			Signal kind	: 0x5A20 (3G)	0x5A21 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：RE PORT状態報告応答(CMT_CPRIF_REPORTSTSRES)					 */
/* 信号種別：0x5A20		方向:TRA->TRIF										 */
/*************************************************************************** */
/* PORT状態詳細  */
typedef struct
{
	USHORT				port_out_sts;			/**< PORT出力状態				 */
	USHORT				port_in_sts;			/**< PORT入力状態				 */
}PORTSTS_INF;

typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	PORTSTS_INF			portsts_inf;			/**< PORT状態詳細				 */
}__attribute__ ((packed)) CMT_CPRIF_REPORTSTSRES ;

/**
 * @brief	RE card diagnosis request
 * @note	MESSAGE		: RE card diagnosis request \n
 * 			Signal kind	: 0x5B10 (3G)	0x5B11 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REカード診断要求(CMT_CPRIF_RECRDDIAREQ)						 */
/* 信号種別：0x5B10		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				slot_no;				/**< スロット番号				 */
	
} __attribute__ ((packed)) CMT_CPRIF_RECRDDIAREQ ;

/**
 * @brief	RE card diagnosis response
 * @note	MESSAGE		: RE card diagnosis response \n
 * 			Signal kind	: 0x5B20 (3G)	0x5B21 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：REカード診断応答(CMT_CPRIF_RECRDDIARES)						 */
/* 信号種別：0x5B20		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				slot_no;				/**< スロット番号				 */
	USHORT				dia_result;				/**< 診断結果					 */
} __attribute__ ((packed)) CMT_CPRIF_RECRDDIARES ;

/**
 * @brief	RE trouble log get request
 * @note	MESSAGE		: RE trouble log get request \n
 * 			Signal kind	: 0x5C10 (3G)	0x5C11 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：RE障害ログ取得要求(CMT_CPRIF_REOBSLOGREQ)						 */
/* 信号種別：0x5C10		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				comsec;					/**< RE共通部/REセクタ部指定	 */
	UCHAR				filename[256];			/**< 障害ログファイル名		 */
}__attribute__ ((packed)) CMT_CPRIF_REOBSLOGREQ ;

/**
 * @brief	RE trouble log get response
 * @note	MESSAGE		: RE trouble log get response \n
 * 			Signal kind	: 0x5C20 (3G)	0x5C21 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE障害ログ取得応答(CMT_CPRIF_REOBSLOGRES)						 */
/* 信号種別：0x5C20		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	UCHAR				filename[256];			/**< ファイル名				 */
	UINT				datsize;				/**< トータルデータサイズ		 */
}__attribute__ ((packed)) CMT_CPRIF_REOBSLOGRES ;

/**
 * @brief	RE trouble log get stop request
 * @note	MESSAGE		: RE trouble log get stop request \n
 * 			Signal kind	: 0x5D10 (3G)	0x5D11 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：RE障害ログ取得中止要求(CMT_CPRIF_REOBLOGSTPREQ)				 */
/* 信号種別：0x5D10		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_REOBLOGSTPREQ ;

/**
 * @brief	RE trouble log get stop response
 * @note	MESSAGE		: RE trouble log get stop response \n
 * 			Signal kind	: 0x5D20 (3G)	0x5D21 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：RE障害ログ取得中止応答(CMT_CPRIF_REOBLOGSTPRES)				 */
/* 信号種別：0x5D20		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_REOBLOGSTPRES ;

/**
 * @brief	external equipment data sending request
 * @note	MESSAGE		: external equipment data sending request \n
 * 			Signal kind	: 0x9110 (3G)	0x9111 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：外部装置データ送信要求(CMT_CPRIF_EXTDATSNDREQ)					 */
/* 信号種別：0x9110		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				extdev_no;				/**< 外部装置番号				 */
	USHORT				snd_byte;				/**< 送信バイト数				 */
	UCHAR				snd_msg[2048];			/**< 送信メッセージ			 */
}__attribute__ ((packed)) CMT_CPRIF_EXTDATSNDREQ ;

/**
 * @brief	external equipment data sending response
 * @note	MESSAGE		: external equipment data sending response \n
 * 			Signal kind	: 0x9120 (3G)	0x9121 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：外部装置データ送信応答(CMT_CPRIF_EXTDATSNDRES)					 */
/* 信号種別：0x9120		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				extdev_no;				/**< 外部装置番号				 */
}__attribute__ ((packed)) CMT_CPRIF_EXTDATSNDRES ;

/**
 * @brief	external equipment data receiving request
 * @note	MESSAGE		: external equipment data receiving request \n
 * 			Signal kind	: 0x9130 (3G)	0x9131 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：外部装置データ受信要求(CMT_CPRIF_EXTDATRCVREQ)					 */
/* 信号種別：0x9130		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				extdev_no;				/**< 外部装置番号				 */
	USHORT				sndbyte_num;			/**< 送信バイト数				 */
	UCHAR				sndmsg[2048];			/**< 送信メッセージ			 */
}__attribute__ ((packed)) CMT_CPRIF_EXTDATRCVREQ ;

/**
 * @brief	external equipment data receiving response
 * @note	MESSAGE		: external equipment data receiving response \n
 * 			Signal kind	: 0x9140 (3G)	0x9141 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：外部装置データ受信応答(CMT_CPRIF_EXTDATRCVRES)					 */
/* 信号種別：0x9140		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				extdev_no;				/**< 外部装置番号				 */
}__attribute__ ((packed)) CMT_CPRIF_EXTDATRCVRES ;

/**
 * @brief	MT address set request
 * @note	MESSAGE		: MT address set request \n
 * 			Signal kind	: 0x9210 (3G)	0x9211 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：MTアドレス設定要求(CMT_CPRIF_MTADDSETREQ)						 */
/* 信号種別：0x9210		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				tgtmt;					/**< 対象MT					 */
	UCHAR				ipadd_1;				/**< IPアドレス＃1			 */
	UCHAR				ipadd_2;				/**< IPアドレス＃2			 */
	UCHAR				ipadd_3;				/**< IPアドレス＃3			 */
	UCHAR				ipadd_4;				/**< IPアドレス＃4			 */
	UCHAR				sbnetmsk_1;				/**< サブネットマスク＃1		 */
	UCHAR				sbnetmsk_2;				/**< サブネットマスク＃2		 */
	UCHAR				sbnetmsk_3;				/**< サブネットマスク＃3		 */
	UCHAR				sbnetmsk_4;				/**< サブネットマスク＃4		 */
	USHORT				cmdtrsport;				/**< コマンド転送用ポート番号	 */
	USHORT				filtrsport;				/**< ファイル転送用ポート番号	 */
	USHORT				portno;					/**< ポート番号				 */
}__attribute__ ((packed)) CMT_CPRIF_MTADDSETREQ ;

/**
 * @brief	MT address set response
 * @note	MESSAGE		: MT address set response \n
 * 			Signal kind	: 0x9220 (3G)	0x9221 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：MTアドレス設定応答(CMT_CPRIF_MTADDSETRES)						 */
/* 信号種別：0x9220		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_MTADDSETRES ;

/**
 * @brief	FLD-MT connect request
 * @note	MESSAGE		: FLD-MT connect request \n
 * 			Signal kind	: 0x9310 (3G)	0x9311 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MT接続要求(CMT_CPRIF_FLDMTCNCREQ)							 */
/* 信号種別：0x9310		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
} __attribute__ ((packed)) CMT_CPRIF_FLDMTCNCREQ;

/**
 * @brief	FLD-MT connect response
 * @note	MESSAGE		: FLD-MT connect response \n
 * 			Signal kind	: 0x9320 (3G)	0x9321 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MT接続応答(CMT_CPRIF_FLDMTCNCRES)							 */
/* 信号種別：0x9320		方向:TRIF->TRA										 */
/**************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	
} __attribute__ ((packed)) CMT_CPRIF_FLDMTCNCRES ;

/**
 * @brief	FLD-MT command data sending request 
 * @note	MESSAGE		: FLD-MT command data sending request  \n
 * 			Signal kind	: 0x9370 (3G)	0x9371 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：FLD-MTデータコマンド転送送信要求(CMT_CPRIF_FLDCOMSNDREQ)		 */
/* 信号種別：0x9370		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				sndalldat_num;			/**< 全送信データ数			 */
	USHORT				snddatseq_no;			/**< 送信データ順序番号		 */
	USHORT				sndbyte_num;			/**< 送信バイト数				 */
	UCHAR				snddat[10240];			/**< 送信データ				 */
}__attribute__ ((packed)) CMT_CPRIF_FLDCOMSNDREQ ;

/**
 * @brief	FLD-MT command data sending response 
 * @note	MESSAGE		: FLD-MT command data sending response  \n
 * 			Signal kind	: 0x9380 (3G)	0x9381 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：FLD-MTデータコマンド転送送信応答(CMT_CPRIF_FLDCOMSNDRES)		 */
/* 信号種別：0x9380		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				sndalldat_num;			/**< 全送信データ数			 */
	USHORT				snddatseq_no;			/**< 送信データ順序番号		 */
}__attribute__ ((packed)) CMT_CPRIF_FLDCOMSNDRES ;

/**
 * @brief	FLD-MT command data receiving request 
 * @note	MESSAGE		: FLD-MT command data receiving request  \n
 * 			Signal kind	: 0x9390 (3G)	0x9391 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MTデータコマンド転送受信要求(CMT_CPRIF_FLDCOMRCVREQ)		 */
/* 信号種別：0x9390		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				rcvalldat_num;			/**< 全受信データ数			 */
	USHORT				rcvdatseq_no;			/**< 受信データ順序番号		 */
	USHORT				rcvbyte_num;			/**< 受信バイト数				 */
	UCHAR				rcvdat[10240];			/**< 受信データ				 */
}__attribute__ ((packed)) CMT_CPRIF_FLDCOMRCVREQ ;

/**
 * @brief	FLD-MT command data receiving response 
 * @note	MESSAGE		: FLD-MT command data receiving response  \n
 * 			Signal kind	: 0x93A0 (3G)	0x93A1 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：FLD-MTデータコマンド転送受信応答(CMT_CPRIF_FLDCOMRCVRES)		 */
/* 信号種別：0x93A0		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
	USHORT				rcvalldat_num;			/**< 全受信データ数			 */
	USHORT				rcvdatseq_no;			/**< 受信データ順序番号		 */
}__attribute__ ((packed)) CMT_CPRIF_FLDCOMRCVRES ;

/**
 * @brief	FLD-MT file data sending request 
 * @note	MESSAGE		: FLD-MT file data sending request  \n
 * 			Signal kind	: 0x93B0 (3G)	0x93B1 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MTデータファイル転送送信要求(CMT_CPRIF_FLDFILSNDREQ)		 */
/* 信号種別：0x93B0		方向:TRIF->MDIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				sndbyte_num;			/**< 送信バイト数				 */
	UCHAR				snddat[4096];			/**< 送信データ				 */
}__attribute__ ((packed)) CMT_CPRIF_FLDFILSNDREQ ;

/**
 * @brief	FLD-MT file data sending request 
 * @note	MESSAGE		: FLD-MT file data sending request  \n
 * 			Signal kind	: 0x93B0 (3G)	0x93B1 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MTデータファイル転送送信要求(CMT_CPRIF_FLDFILSNDREQ_S3G)		 */
/* 信号種別：0x93B1		方向:TRIF->MDIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				protocol_type;
	USHORT				data_type;
	USHORT				sndbyte_num;			/**< 送信バイト数				 */
	UCHAR				snddat[4096];			/**< 送信データ				 */
}__attribute__ ((packed)) CMT_CPRIF_FLDFILSNDREQ_S3G ;		

/**
 * @brief	FLD-MT file data sending response 
 * @note	MESSAGE		: FLD-MT file data sending response  \n
 * 			Signal kind	: 0x93C0 (3G)	0x93C1 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MTデータファイル転送送信応答(CMT_CPRIF_FLDFILSNDRES)		 */
/* 信号種別：0x93C0		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FLDFILSNDRES ;

/**
 * @brief	FLD-MT file data receiving request 
 * @note	MESSAGE		: FLD-MT file data receiving request  \n
 * 			Signal kind	: 0x93D0 (3G)	0x93D1 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MTデータファイル転送受信要求(CMT_CPRIF_FLDFILRCVREQ)		 */
/* 信号種別：0x93D0		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				rcvbyte_num;			/**< 受信バイト数				 */
	UCHAR				rcvdat[4096];			/**< 受信データ				 */
}__attribute__ ((packed)) CMT_CPRIF_FLDFILRCVREQ ;

/**
 * @brief	FLD-MT file data receiving request 
 * @note	MESSAGE		: FLD-MT file data receiving request  \n
 * 			Signal kind	: 0x93D1 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MTデータファイル転送受信要求(CMT_CPRIF_FLDFILRCVREQ)		 */
/* 信号種別：0x93D1		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				protocol_type;
	USHORT				ftpdata_type;			/**< FTP データ種別				*/
	USHORT				rcvbyte_num;			/**< 受信バイト数				 */
	UCHAR				rcvdat[4096];			/**< 受信データ				 */
}__attribute__ ((packed)) CMT_CPRIF_FLDFILRCVREQ_S3G ;


/**
 * @brief	FLD-MT file data receiving response 
 * @note	MESSAGE		: FLD-MT file data receiving response  \n
 * 			Signal kind	: 0x93E0 (3G)	0x93E1 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：FLD-MTデータファイル転送受信応答(CMT_CPRIF_FLDFILRCVRES)		 */
/* 信号種別：0x93E0		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FLDFILRCVRES ;

/**
 * @brief	FLD-MT release request 
 * @note	MESSAGE		: FLD-MT release request  \n
 * 			Signal kind	: 0x9330 (3G)	0x9331 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MT接続解放要求(CMT_CPRIF_FLMTCNCRELREQ)					 */
/* 信号種別：0x9330		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
} __attribute__ ((packed)) CMT_CPRIF_FLMTCNCRELREQ;

/**
 * @brief	FLD-MT release response 
 * @note	MESSAGE		: FLD-MT release response  \n
 * 			Signal kind	: 0x9340 (3G)	0x9341 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：FLD-MT接続解放応答(CMT_CPRIF_FLMTCNCRLSRES)					 */
/* 信号種別：0x9340		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
} __attribute__ ((packed)) CMT_CPRIF_FLMTCNCRELRES ;

/**
 * @brief	FLD-MT force release request  
 * @note	MESSAGE		: FLD-MT force release request   \n
 * 			Signal kind	: 0x9350 (3G)	0x9351 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：FLD-MT接続強制解放要求(CMT_CPRIF_FLMTCMPRELREQ)				 */
/* 信号種別：0x9350		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
} __attribute__ ((packed)) CMT_CPRIF_FLMTCMPRELREQ;

/**
 * @brief	FLD-MT force release response  
 * @note	MESSAGE		: FLD-MT force release response   \n
 * 			Signal kind	: 0x9360 (3G)	0x9361 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：FLD-MT接続強制解放応答(CMT_CPRIF_FLMTCMPRELRES)				 */
/* 信号種別：0x9360		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FLMTCMPRELRES ;

/**
 * @brief	SV-MT connect request  
 * @note	MESSAGE		: SV-MT connect request   \n
 * 			Signal kind	: 0x9410 (3G)	0x9411 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：SV-MT接続要求(CMT_CPRIF_SVMTCNTREQ)							 */
/* 信号種別：0x9410		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTCNTREQ ;

/**
 * @brief	SV-MT connect response  
 * @note	MESSAGE		: SV-MT connect response   \n
 * 			Signal kind	: 0x9420 (3G)	0x9421 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MT接続応答(CMT_CPRIF_SVMTCNTRES)							 */
/* 信号種別：0x9420		方向:TRA->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTCNTRES ;

/**
 * @brief	SV-MT data sending request  
 * @note	MESSAGE		: SV-MT data sending request   \n
 * 			Signal kind	: 0x9470 (3G)	0x9471 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MTデータ送信要求(CMT_CPRIF_SVMTDATSNDREQ)					 */
/* 信号種別：0x9470		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				snd_bytnum;				/**< 送信バイト数				 */
	UCHAR				snd_msg[10240];			/**< 送信メッセージ			 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTDATSNDREQ ;

/**
 * @brief	SV-MT data sending response  
 * @note	MESSAGE		: SV-MT data sending response   \n
 * 			Signal kind	: 0x9480 (3G)	0x9481 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：SV-MTデータ送信応答(CMT_CPRIF_SVMTDATSNDRES)					 */
/* 信号種別：0x9480		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTDATSNDRES ;

/**
 * @brief	SV-MT data receiving request  
 * @note	MESSAGE		: SV-MT data receiving request   \n
 * 			Signal kind	: 0x9490 (3G)	0x9491 (S3G) \n
 * 			Direction	: TRA ->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MTデータ受信要求(CMT_CPRIF_SVMTDATRCVREQ)					 */
/* 信号種別：0x9490		方向：TRA->TRIF									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				rcv_bytnum;				/**< 受信バイト数				 */
	UCHAR				rcv_msg[10240];			/**< 受信メッセージ			 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTDATRCVREQ ;

/**
 * @brief	SV-MT data receiving response  
 * @note	MESSAGE		: SV-MT data receiving response   \n
 * 			Signal kind	: 0x94A0 (3G)	0x94A1 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MTデータ受信応答(CMT_CPRIF_SVMTDATRCVRES)					 */
/* 信号種別：0x94A0		方向：TRIF->TRA									 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTDATRCVRES ;

/**
 * @brief	SV-MT release request  
 * @note	MESSAGE		: SV-MT release request   \n
 * 			Signal kind	: 0x9430 (3G)	0x9431 (S3G) \n
 * 			Direction	: TRIF ->TRA  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MT接続解放要求(CMT_CPRIF_SVMTCNTRELREQ)						 */
/* 信号種別：0x9430		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTCNTRELREQ ;

/**
 * @brief	SV-MT release response  
 * @note	MESSAGE		: SV-MT release response   \n
 * 			Signal kind	: 0x9440 (3G)	0x9441 (S3G) \n
 * 			Direction	:  TRA-> TRIF \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MT接続解放応答(CMT_CPRIF_SVMTCNTRELRES)						 */
/* 信号種別：0x9440		方向:MDIF->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTCNTRELRES ;

/**
 * @brief	SV-MT force release request  
 * @note	MESSAGE		: SV-MT force release request   \n
 * 			Signal kind	: 0x9450 (3G)	0x9451 (S3G) \n
 * 			Direction	:  TRIF->TRA  \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：SV-MT接続強制解放要求(CMT_CPRIF_SVMTFORRELREQ)					 */
/* 信号種別：0x9450		方向:TRIF->TRA										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTFORRELREQ ;

/**
 * @brief	SV-MT force release response  
 * @note	MESSAGE		: SV-MT force release response   \n
 * 			Signal kind	: 0x9460 (3G)	0x9461 (S3G) \n
 * 			Direction	:  TRA->TRIF  \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：SV-MT接続強制解放応答(CMT_CPRIF_SVMTFORRELRES)					 */
/* 信号種別：0x9460		方向:MDIF->TRIF										 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_SVMTFORRELRES ;

/**
 * @brief	Maker MT data sending  
 * @note	MESSAGE		: Maker MT data sending   \n
 * 			Signal kind	: 0x9500 (3G)	0x9501 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：メーカMTデータ送信(CMT_CPRIF_MKMTDATSND)						 */
/* 信号種別：0x9500		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
	UINT				mt_length;				/**< lenght of mt_signal + length of mt_data  */
	UINT				mt_signal;				/**< OPT-MT信号種別			 */
	UCHAR				mt_data[CMD_NUM4];			/**< OPT-MTデータ				 */
}__attribute__ ((packed)) CMT_CPRIF_MKMTDATSND ;

/**
 * @brief	Maker MT data connection request  
 * @note	MESSAGE		: Maker MT connection request   \n
 * 			Signal kind	: 0x9510 (3G)	0x9511 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：メーカMT接続要求(CMT_CPRIF_MKMTCNTREQ)							 */
/* 信号種別：0x9510		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
}__attribute__ ((packed)) CMT_CPRIF_MKMTCNTREQ ;

/**
 * @brief	Maker MT data connection response  
 * @note	MESSAGE		: Maker MT connection response   \n
 * 			Signal kind	: 0x9520 (3G)	0x9521 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：メーカMT接続応答(CMT_CPRIF_MKMTCNTRES) 						 */
/* 信号種別：0x9520		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
	UINT				result;					/**< 応答結果					 */
	UINT				NG_code;				/**< NG code					 */
}__attribute__ ((packed)) CMT_CPRIF_MKMTCNTRES ;

/**
 * @brief	Maker MT data release request  
 * @note	MESSAGE		: Maker MT release request   \n
 * 			Signal kind	: 0x9530 (3G)	0x9531 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */

/*************************************************************************** */
/* MESSAGE ：メーカMT接続解放要求(CMT_CPRIF_MKMTCNTRELREQ)					 */
/* 信号種別：0x9530		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
	UINT				cause;					/**< reserve					 */
}__attribute__ ((packed)) CMT_CPRIF_MKMTCNTRELREQ ;

/**
 * @brief	Maker MT data release response  
 * @note	MESSAGE		: Maker MT release response   \n
 * 			Signal kind	: 0x9540 (3G)	0x9541 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE ：メーカMT接続解放応答(CMT_CPRIF_MKMTCNTRELRES)					 */
/* 信号種別：0x9540		方向:TRIF->MDIF、MDIF->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
	UINT				result;					/**< 応答結果					 */
	UINT				NG_code;				/**< NG code					 */
	
}__attribute__ ((packed)) CMT_CPRIF_MKMTCNTRELRES ;

/**
 * @brief	Maker MT data force release request  
 * @note	MESSAGE		: Maker MT force release request   \n
 * 			Signal kind	: 0x9550 (3G)	0x9551 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：メーカMT接続強制解放要求(CMT_CPRIF_MKMTFORRELREQ) 				 */
/* 信号種別：0x9550		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
	UINT				cause;					/**< reserve					 */
}__attribute__ ((packed)) CMT_CPRIF_MKMTFORRELREQ ;

/**
 * @brief	Maker MT data force release response  
 * @note	MESSAGE		: Maker MT force release response   \n
 * 			Signal kind	: 0x9560 (3G)	0x9561 (S3G) \n
 * 			Direction	:  TRA->TRIF TRIF->TRA \n
 * 
 */


/*************************************************************************** */
/* MESSAGE ：メーカMT接続強制解放応答(CMT_CPRIF_MKMTFORRELRES) 				 */
/* 信号種別：0x9560		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				mkmt_kind;				/**< メーカMT種別				 */
	UINT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_MKMTFORRELRES ;

/*************************************************************************** */
/* MESSAGE ：(CMT_CPRIF_FTPPORTSETREQ) 				 */
/* 信号種別：0x9611		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				portnum;			/**< メーカMT種別				 */
}__attribute__ ((packed)) CMT_CPRIF_FTPPORTSETREQ ;

/*************************************************************************** */
/* MESSAGE ：(CMT_CPRIF_FTPPORTSETRES) 				 */
/* 信号種別：0x9612		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FTPPORTSETRES ;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*************************************************************************** */
/* MESSAGE ：(CMT_CPRIF_FTPADDSETTOREQ) 				 */
/* 信号種別：0x9990		方向:TRIF->TRA、TRA->TRIF							 */
/*************************************************************************** */
typedef struct
{
	USHORT				signal_kind;			/**< 信号種別					 */
	USHORT				result;					/**< 応答結果					 */
}__attribute__ ((packed)) CMT_CPRIF_FTPADDSETTOREQ ;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	RE device management table
 * @note	RE device management table
 * 
  */
typedef struct{
	USHORT				mkr_nm;								/**< Maker name   */
	USHORT				bts_type;							/**< BTS type   */
	USHORT				re_type;							/**< RE type   */
	USHORT				dev_kind;							/**< device kind (TBD)   */
	USHORT				dev_no;								/**< device number (TBD)   */
	USHORT				div_type;							/**< Diversity type (3G use)   */
	USHORT				pwr_type;							/**< power type   */
	USHORT				pwr_class_unit;						/**< power class unit   */
	USHORT				pwr_class0;							/**< power class (Br0)   */
	USHORT				pwr_class1;							/**< power class (Br1)   */
	USHORT				pwr_class2;							/**< power class (Br2)   */
	USHORT				pwr_class3;							/**< power class (Br3)   */
	USHORT				crp_frq;							/**< corresponding frequency area   */
	USHORT				max_bandwidth;						/**< Max bandwidth   */
	USHORT				max_tx_ant_no;						/**< Max TX antenna number   */
	USHORT				fii_type;							/**< 800MHz filter type   */
	USHORT				cmx_2gcarcnt;						/**< 2GHz frequency carrier number   */
	USHORT				cmx_800mcarcnt;						/**< 800MHz frequency carrier number   */
	USHORT				cmx_17gcarcnt;						/**< 1.7GHz frequency carrier number   */
	USHORT				cmx_trxinf_typ;						/**< RE side TRX-INF type   */
	USHORT				cmx_15gcarcnt;						/**< 1.5GHz frequency carrier number   */
	USHORT				cmx_35gcarcnt;						/**< 3.5GHz frequency carrier number   */
	USHORT				mca_filt;
}CMT_REDEVCOMPTBL;

/*************************************************************************** */
/* 																			 */
/*	No.4	Common structure for Common function and individual function	 */
/* 																			 */
/*************************************************************************** */

/*************************************************************************** */
/* RTCデータ型(cm_RtcSet関連)												 */
/*************************************************************************** */
typedef struct
{
	USHORT			year  ;					/**< 年							 */
	UCHAR			month ;					/**< 月							 */
	UCHAR			day   ;					/**< 日							 */
	UCHAR			hour  ;					/**< 時							 */
	UCHAR			min   ;					/**< 分							 */
	UCHAR			sec   ;					/**< 秒							 */
	UCHAR			msec  ;					/**< ミリ秒						 */
	
} DATETIME_T ;

									/**< [0]:年、[1]:月日、[2]:時分、[3]:秒	 */
typedef struct
{
	USHORT			rtc[4] ;
	
} RTCSTYLE_T ;


typedef union 
{
	DATETIME_T	datetime;
	RTCSTYLE_T	rtc_style;
	
}CMT_RTCDATA_T;


/* ========================================================================  */
/* LAPB N/E switch management table											 */
/* ========================================================================  */
typedef struct
{
	UCHAR			status;
	UCHAR			vr;
	UCHAR			vs;
	UCHAR			va;
	UCHAR			rej_cond;
	UCHAR			per_bsy;
	UCHAR			retry_cnt;
	UCHAR			t1_flg;
	UCHAR			t2_cnt;
	UCHAR			t3_flg;
	UCHAR			t4_flg;
	UCHAR			dummy;
}LBT_NESTKTBL;

#if 0



/* ========================================================================  */
/* 障害ログ退避領域(1Mbyte)													 */
/* ========================================================================  */
typedef struct
{
	/* =====================================================================  */
	/* 障害ログヘッダ(48byte)												  */
	/* =====================================================================  */
	UCHAR	separator[CMD_LOG_SEPANUM];		/**< セパレータ					  */
	UINT	areaSize;						/**< エリアサイズ				  */
	UINT	cardNo;							/**< カード番号					  */
	UCHAR	lmVer[CMD_NUM16];				/**< LMバージョン				  */
	UCHAR	time[CMD_NUM8];					/**< 時刻情報					  */
	USHORT	almFct;							/**< ALM要因						  */
	USHORT	almFlg;							/**< ALM要因						  */
#ifndef OPT_RRE_1CHP
	UCHAR	reserve[CMD_NUM8];				/**< リザーブ					*/
#else
	UINT	device_kind;					/**< device type				  */
	UCHAR	reserve[CMD_NUM4];				/**< リザーブ					  */
#endif
}CMT_ALMLOG_HEAD_T ;

typedef struct
{
CMT_ALMLOG_HEAD_T	header ;				/**< ヘッダ						  */
	/* =====================================================================  */
	/* ログ情報格納領域(1024 * 1024 - 48 = 1048528byte)						  */
	/* =====================================================================  */
	UCHAR	Log[1024 * 1024 - 48] ;			/**< ログ情報格納領域				  */
}CMT_ALMLOG_T ;

/* ========================================================================  */
/* 障害ログ設定テーブル														 */
/* ========================================================================  */
typedef struct
{
	UINT	set_kind;						/**< ログ収集種別					 */
	UCHAR	log_char[CMD_NUM16];			/**< ログ名称						 */
#ifndef OPT_RRE_1CHP
	CHAR	table_name[CMD_NUM32];			/**< テーブル名					 */
	UINT	table_address;					/**< テーブルアドレス				 */
	UINT	table_size;						/**< テーブルサイズ				 */
#else
	UINT	table_size;						/**< テーブルサイズ				 */
	CHAR	dummy[12];						/**< dummy					 */
	CHAR	table_name[CMD_NUM32];			/**< テーブル名					 */
	UINT	table_address;					/**< テーブルアドレス				 */
#endif
}CMT_ALMLOGSET_T ;

/* ========================================================================  */
/* 障害ログ管理領域															 */
/* ========================================================================  */
typedef struct
{
	UINT	EnableFlag ;					/**< 有効フラグ					 */
	UINT	Length ;						/**< 障害ログ総データ長			 */
	UINT	Reserve[2] ;					/**< 予備							 */
	
} CMT_ALMLOGMNG2_T ;

typedef struct
{
	UINT				LatestBank ;		/**< 最新ログ格納面				 */
	UINT				LatestAdr ;			/**< 最新ログ格納アドレス			 */
	UINT				SendFlg ;			/**< 送信完了フラグ				 */
	UINT				WriteCnt ;			/**< 書込カウンタ					 */
	CMT_ALMLOGMNG2_T	Bank[3] ;			/**< 個別情報						 */
	DATETIME_T			DateTime[32] ;		/**< 事象発生日時情報				 */
	
}CMT_ALMLOGMNG_T ;
#endif
/* ========================================================================  */
/* 状態フラグ管理テーブル													 */
/* ========================================================================  */
typedef struct
{
	USHORT		rst_flg;						/**< リセット制御中フラグ		 */
	USHORT		ext_rst_flg;					/**< EXTリセット制御中フラグ	 */
	USHORT		dl_flg;							/**< ダウンロード実行中フラグ	 */
	USHORT		log_flg;						/**< 障害ログアクセス中フラグ	 */
	USHORT		eep_flg;						/**< EEPROMアクセス中フラグ	 */
	USHORT		dummy;							/**< dummy領域				 */
} CMT_FLGMNGTBL;

/*************************************************************************** */
/* 																			 */
/*		No.5		タスク間インタフェースフレーム定義						 */
/* 																			 */
/*************************************************************************** */

/*!
 * @name メッセージ共通ヘッダ
 * @note メッセージ共通ヘッダ定義:どのprocessでどのようなmessageを送信する場合でも本ヘッダを使用すること
 * @{
 */
typedef struct{
	UINT		uiEventNo;					/**< イベント番号						*/
	UINT		uiSignalkind;				/**< SignalKind(EventIDが重複しない場合はDon't care)*/
	UINT		uiDstPQueueID;				/**< 最終送信先Process Queue ID			*/
	UINT		uiDstTQueueID;				/**< 最終送信先Thread  Queue ID			*/
	UINT		uiSrcPQueueID;				/**< 送信元    Process Queue ID			*/
	UINT		uiSrcTQueueID;				/**< 送信元    Thread  Queue ID			*/
	UINT		uiLength;					/**< Length(Head部 + Data部)			*/
	UINT		uiKeyID;					/**< 通信用共有メモリ管理用KEYID		*/
}CMT_TSKIF_HEAD;
/* @} */


/* ========================================================================  */
/* MESSAGE :初期化要求(CMT_TSKIF_INIREQ)									 */
/* 起動要因:0x0101															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */

/* ========================================================================  */
/* MESSAGE :初期化応答(CMT_TSKIF_INIRES)									 */
/* 起動要因:0x0102															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */

/**
 * @brief	L2 DEACT Request    
 * @note	MESSAGE		: L2 DEACT Request     \n
 * 			Event number: 0x0201 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L2 DEACT依頼(CMT_TSKIF_L2DEACTREQ)								 */
/* 起動要因:0x0201															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 head;
	USHORT			link_num;		/**< CPRI番号					 */
	UCHAR			dummy1;			/**< ダミー領域	1			 */
	UCHAR			dummy2;			/**< ダミー領域	2			 */
}CMT_TSKIF_L2DEACTREQ;

/**
 * @brief	Data Link Establishment  Request     
 * @note	MESSAGE		: Data Link Establishment  Request      \n
 * 			Event number: 0x0203 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :データリンク設定要求(CMT_TSKIF_L2ESTREQ)						 */
/* 起動要因:0x0203															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 head;
	USHORT			link_num;		/**< CPRI番号					 */
	UCHAR			dummy1;			/**< ダミー領域	1			 */
	UCHAR			dummy2;			/**< ダミー領域	2			 */
}CMT_TSKIF_L2ESTREQ;
	

/**
 * @brief	L2 STOP Request       
 * @note	MESSAGE		: L2 STOP Request        \n
 * 			Event number: 0x0205 \n
 * 
 */
 

/*************************************************************************** */
/* MESSAGE :L2 STOP依頼(CMT_TSKIF_L2STPREQ)									 */
/* 起動要因:0x0205															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 	head;
	USHORT			link_num;
	UCHAR			dummy1;
	UCHAR			dummy2;
}CMT_TSKIF_L2STPREQ;

/**
 * @brief	L3 Test Start Notice        
 * @note	MESSAGE		: L3 Test Start Notice         \n
 * 			Event number: 0x0207 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L3 test 開始通知(CMT_TSKIF_L3MODESTRNTC)						 */
/* 起動要因:0x0207															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 	head;
	USHORT			link_num;
	UCHAR			dummy1;
	UCHAR			dummy2;
}CMT_TSKIF_L3MODESTRNTC;	

/**
 * @brief	L3 Test Stop Notice         
 * @note	MESSAGE		: L3 Test Stop Notice          \n
 * 			Event number: 0x0209 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L3 test 停止通知(CMT_TSKIF_L3MODESTPNTC)						 */
/* 起動要因:0x0209															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 	head;
	USHORT			link_num;
	UCHAR			dummy1;
	UCHAR			dummy2;
}CMT_TSKIF_L3MODESTPNTC;

 

/**
 * @brief	CPRI Disconnection Notice          
 * @note	MESSAGE		: CPRI Disconnection Notice           \n
 * 			Event number: 0x020B \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :CPRIリンク断通知(CMT_TSKIF_CPRIDSCNTC)							 */
/* 起動要因:0x020B															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			 */
	USHORT				cpri_no;				/**< CPRI番号					 */
	USHORT				dislink_inf;			/**< CPRIリンク断詳細要因		 */
	USHORT				system_type;		/**< system type */
	USHORT				dummy;				/**< dummy space */
}CMT_TSKIF_CPRIDSCNTC;			

/*************************************************************************** */
/* MESSAGE : 	CPRIリンクActive / Negative変化通知 (CMT_TSKIF_CPRIACTNEGNTC) */
/* 起動要因: 	0x020F								 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;			/**< タスク共通ヘッダ		  */
	USHORT				cpirclsinfo;	/**< Active / Negative情報  */
	USHORT				systemtype;		/**< system type		  */
}CMT_TSKIF_CPRIACTNEGNTC;		

/* ========================================================================  */
/* MESSAGE :L2リンクstate通知(CMD_TSKIF_L2STANTC)						 */
/* 起動要因:なし														 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	UINT				state;				    /**< l2 state				 */
} CMT_TSKIF_L2STANTC;	

/* ========================================================================  */
/* MESSAGE :L3リンクstate通知(CMT_TSKIF_L3STATENTC)						 */
/* 起動要因:0x0211															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			 */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT				link_num;               /**< CPRI link number */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT				system_type;		    /**< system type  */
	USHORT				onOff;		            /**< onOff*/
}CMT_TSKIF_L3STATENTC;

/* ========================================================================  */
/* MESSAGE :再設定応答(CMD_TSKIF_RE_SET_RSP)								 */
/* 起動要因:0x0213															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			 */
	USHORT				result;					/**< 応答結果					 */
}CMT_TSKIF_RE_SET_RSP;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 			Event number: 0x0301 \n
 * 
 */

/*************************************************************************** */
/* MESSAGE :CPRI信号受信通知(CMT_TSKIF_CPRIRCVNTC)							 */
/* 起動要因:0x0301															 */
/*************************************************************************** */
/* CPRI信号受信情報部  */
typedef struct
{
	USHORT						link_num;		/**< CPRI番号					 */
	UCHAR						dummy1;			/**< ダミー領域	1			 */
	UCHAR						dummy2;			/**< ダミー領域	2			 */
	UINT					i_link;				/**< I queue pointer			 */
	UINT					length;				/**< buffer length			 */
	UINT					disp;				/**< buffer offset sizes		 */
	UCHAR					Padding1;			/**< Padding1					 */
	UCHAR					Padding2;			/**< Padding2					 */
	UCHAR					Address;			/**< HDLC frame address area	 */
	UCHAR					Control;			/**< HDLC frame control area	 */
}CMT_CPRISIGRCV_INF;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Signal getting) \n
 * 			Event number: 0x0301 \n
 * 
 */

/* CPRI信号受信通知(信号種別取得用)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	USHORT					signal_kind;		/**< 信号種別					 */
	USHORT					dummy;				/**< dummy領域				 */
} CMT_TSKIF_CPRIRCV_SIGNALGET;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (health check request) \n
 * 			Event number: 0x0301 \n
 * 
 */

/* CPRI信号受信通知(ヘルスチェック要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_HCREQ			cpridat_hcreq;		/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_HCREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (health check response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ヘルスチェック応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_HCRES			cpridat_hcres;		/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_HCRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE reset request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REリセット要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_RERSTREQ		cpridat_rerstreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RERSTREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE reset response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REリセット応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_RERSTRES		cpridat_rerstres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RERSTRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE time set indication) \n
 * 			Event number: 0x0301 \n
 * 
 */

/* CPRI信号受信通知(RE時刻設定要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RETIMSETREQ	cpridat_retimsetreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RETIMSETREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE common link establish request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE共通部リンク確立要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_LINKSTSREQ	cpridat_linkstsreq; /**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_LINKSTSREQ ;


/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE common link establish response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE共通部リンク確立応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_LINKSTSRES	cpridat_linkstsres; /**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_LINKSTSRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (firmware information report request) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(ファームウェアファイル報告要求)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_FIRMFILEREQ	cpridat_firmfilereq;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FIRMFILEREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (firmware information report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ファームウェアファイル報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_FIRMFILERES	cpridat_firmfileres;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FIRMFILERES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file information report notification) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ファイル情報報告通知)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_FILEINFREQ	cpridat_fileinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FILEINFREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file information report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ファイル情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_FILEINFRES	cpridat_fileinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FILEINFRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ファイルデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_FILEDATSND	cpridat_filedatsnd;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FILEDATSND ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending) \n
 * 			Event number: 0x0301 \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/* CPRI信号受信通知(ファイルデータ送信転送指示)									*/
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			*/
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 	*/
	CMT_CPRIF_FILEDATSNDINT	cpridat_filedatsndint;	/**< CPRI信号データ部		*/
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FILEDATSNDINT ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending complete report notice) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ファイルデータ送信完了報告通知)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FILSNDFINNTC	cpridat_filsndfinntc;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FILSNDFINNTC ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending complete report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(ファイルデータ送信完了報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FILSNDFINRES	cpridat_filsndfinres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FILSNDFINRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (firmware information report request in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(運用中ファームウェアファイル報告要求)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_UFIRMFILEREQ	cpridat_ufirmfilereq;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFIRMFILEREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (firmware information report response in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(運用中ファームウェアファイル報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_UFIRMFILERES	cpridat_ufirmfileres;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFIRMFILERES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (firmware information report response in use state) \n
 * 			Event number: 0x0301 \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/* CPRI信号受信通知(運用中ファームウェアファイル報告応答2)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部			 */
	CMT_CPRIF_UFIRMFILERES2	cpridat_ufirmfileres2;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFIRMFILERES2 ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file information report notification in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(運用中ファイル情報報告通知)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_UFILEINFREQ	cpridat_ufileinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFILEINFREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file information report response in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(運用中ファイル情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_UFILEINFRES	cpridat_ufileinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFILEINFRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(運用中ファイルデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_UFILEDATSND	cpridat_ufiledatsnd;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFILEDATSND ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending in use state) \n
 * 			Event number: 0x0301 \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/* CPRI信号受信通知(運用中ファイルデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_UFILEDATSNDINT	cpridat_ufiledatsndint;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFILEDATSNDINT ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending complete report notice in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(運用中ファイルデータ送信完了報告通知)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_UFILSNDFINNTC	cpridat_ufilsndfinntc;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFILSNDFINNTC ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (file data sending complete report response in use state) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(運用中ファイルデータ送信完了報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_UFILSNDFINRES	cpridat_ufilsndfinres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_UFILSNDFINRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (TRX set request-3G) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(TRX設定要求)	(3G)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_TRXSETREQ		cpridat_trxsetreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXSETREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (TRX set request-S3G) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(TRX設定要求)	(S3G)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_TRXSETREQ_S3G		cpridat_trxsetreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXSET_S3G_REQ ;


/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* CPRI信号受信通知(TRX設定要求)	(S3G)(FDD)									 */
typedef struct
{
	CMT_TSKIF_HEAD				head;				/**< タスク共通ヘッダ		 */
	CMT_CPRISIGRCV_INF			cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_TRXSETREQ_S3G_FDD	cpridat_trxsetreq;	/**< CPRI信号データ部		 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXSET_S3G_FDD_REQ ;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (TRX set response) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(TRX設定応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_TRXSETRES		cpridat_trxsetres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXSETRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (TRX release request) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(TRX解放要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_TRXRELREQ		cpridat_trxrelreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (TRX release response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(TRX解放応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_TRXRELRES		cpridat_trxrelres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXRELRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Toffset report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(Toffset値報告要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_TOFFSETINFREQ	cpridat_toffsetinfreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TOFFSETINFREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Toffset report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(Toffset値報告応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_TOFFSETINFRES	cpridat_toffsetinfres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TOFFSETINFRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (no modulation carrier On/OFF specify request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(無変調キャリア出力ON/OFF指定要求-3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFREQ	cpridat_caronoffreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARONOFFREQ ;


/* CPRI信号受信通知(無変調キャリア出力ON/OFF指定要求-S3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFREQ_S3G	cpridat_caronoffreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (no modulation carrier On/OFF specify response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(無変調キャリア出力ON/OFF指定応答 - 3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFRES	cpridat_caronoffres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARONOFFRES ;

/* CPRI信号受信通知(無変調キャリア出力ON/OFF指定応答 - S3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFRES_S3G	cpridat_caronoffres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARONOFFRES_S3G ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE equipment composition information report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE装置構成情報報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_EQPSTRREQ		cpridat_eqpstrreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_EQPSTRREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE equipment composition information report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE装置構成情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_EQPSTRRES		cpridat_eqpstrres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_EQPSTRRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE slot information report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REスロット情報報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_SLOTINFREQ	cpridat_slotinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SLOTINFREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE slot information report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REスロット情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_SLOTINFRES	cpridat_slotinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SLOTINFRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE status report request) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(RE状態報告要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_RESTSREQ		cpridat_restsreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RESTSREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE status report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE状態報告応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_RESTSRES		cpridat_restsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RESTSRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card information report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード情報報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_CARDINFREQ	cpridat_cardinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARDINFREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card information report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_CARDINFRES	cpridat_cardinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARDINFRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE version report request) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(REバージョン報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_REVERREQ		cpridat_reverreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REVERREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE version report response) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(REバージョン報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_REVERRES		cpridat_reverres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REVERRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card control request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード制御要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECARDCNTREQ	cpridat_recardcntreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RECARDCNTREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card control response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード制御応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECARDCNTRES	cpridat_recardcntres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RECARDCNTRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card status report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード状態報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_CARDSTSREQ	cpridat_cardstsreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARDSTSREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card status report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード状態報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_CARDSTSRES	cpridat_cardstsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARDSTSRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE carrier status report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REキャリア状態報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_CARLSTSREQ	cpridat_carstsreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARLSTSREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE carrier status report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REキャリア状態報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_CARLSTSRES	cpridat_carstsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_CARLSTSRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE port control request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE PORT制御要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTCNTREQ	cpridat_reportcntreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REPORTCNTREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE port control response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
 
/* CPRI信号受信通知(RE PORT制御応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTCNTRES	cpridat_reportcntres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REPORTCNTRES;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE port status report request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE PORT状態報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTSTSREQ	cpridat_reportstsreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REPORTSTSREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE port status report response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE PORT状態報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTSTSRES	cpridat_reportstsres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REPORTSTSRES;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card diagnosis request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード診断要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECRDDIAREQ	cpridat_recrddiareq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RECRDDIAREQ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE card diagnosis response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(REカード診断応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECRDDIARES	cpridat_recrddiares;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_RECRDDIARES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE trouble log get request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE障害ログ取得要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBSLOGREQ	cpridat_reobslogreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REOBSLOGREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE trouble log get response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE障害ログ取得応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBSLOGRES	cpridat_reobslogres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REOBSLOGRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE trouble log get stop request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE障害ログ取得中止要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBLOGSTPREQ	cpridat_reoblogstpreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (RE trouble log get stop response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(RE障害ログ取得中止応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBLOGSTPRES	cpridat_reoblogstpres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_REOBLOGSTPRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (external equipment data sending request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(外部装置データ送信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATSNDREQ	cpridat_extdatsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_EXTDATSNDREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (external equipment data sending response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(外部装置データ送信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATSNDRES	cpridat_extdatsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_EXTDATSNDRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (external equipment data receiving request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(外部装置データ受信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATRCVREQ	cpridat_extdatrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_EXTDATRCVREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (external equipment data receiving response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(外部装置データ受信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATRCVRES	cpridat_extdatrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_EXTDATRCVRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (MT address set request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(MTアドレス設定要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MTADDSETREQ	cpridat_mtaddsetreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MTADDSETREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (MT address set response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(MTアドレス設定応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MTADDSETRES	cpridat_mtaddsetres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MTADDSETRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT connect request) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
 
/* CPRI信号受信通知(FLD-MT接続要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDMTCNCREQ	cpridat_fldmtcncreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDMTCNCREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT connect response) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MT接続応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDMTCNCRES	cpridat_fldmtcncres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDMTCNCRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT command data sending request ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータコマンド転送送信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMSNDREQ	cpridat_fldcomsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT command data sending response  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータコマンド転送送信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMSNDRES	cpridat_fldcomsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDCOMSNDRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT command data receiving request  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータコマンド転送受信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMRCVREQ	cpridat_fldcomrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT command data receiving response  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータコマンド転送受信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMRCVRES	cpridat_fldcomrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDCOMRCVRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT file data sending request  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータファイル転送送信要求 - 3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILSNDREQ	cpridat_fldfilsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDFILSNDREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT file data sending request  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータファイル転送送信要求 - S3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILSNDREQ_S3G	cpridat_fldfilsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT file data sending response  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータファイル転送送信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILSNDRES	cpridat_fldfilsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDFILSNDRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT file data receiving request  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータファイル転送受信要求 - 3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILRCVREQ	cpridat_fldfilrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDFILRCVREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT file data receiving request  ) \n
 * 			Event number: 0x0301 \n
 * 
 */

/* CPRI信号受信通知(FLD-MTデータファイル転送受信要求 - S3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILRCVREQ_S3G	cpridat_fldfilrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT file data receiving response  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MTデータファイル転送受信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILRCVRES	cpridat_fldfilrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLDFILRCVRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT release request  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MT接続解放要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCNCRELREQ	cpridat_flmtcncrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT release response  ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MT接続解放応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCNCRELRES	cpridat_flmtcncrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLMTCNCRELRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT force release request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MT接続強制解放要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCMPRELREQ	cpridat_flmtcmprelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (FLD-MT force release response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(FLD-MT接続強制解放応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCMPRELRES	cpridat_flmtcmprelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_FLMTCMPRELRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT connect request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MT接続要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_SVMTCNTREQ	cpridat_svmtcntreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTCNTREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT connect response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MT接続応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_SVMTCNTRES	cpridat_svmtcntres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTCNTRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT data sending request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MTデータ送信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATSNDREQ	cpridat_svmtdatsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT data sending response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MTデータ送信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATSNDRES	cpridat_svmtdatsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTDATSNDRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT data receiving request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MTデータ受信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATRCVREQ	cpridat_svmtdatrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT data receiving response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MTデータ受信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATRCVRES	cpridat_svmtdatrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTDATRCVRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT release request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MT接続解放要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTCNTRELREQ	cpridat_svmtcntrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT release response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MT接続解放応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTCNTRELRES	cpridat_svmtcntrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTCNTRELRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT force release request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MT接続強制解放要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTFORRELREQ	cpridat_svmtforrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTFORRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (SV-MT force release response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(SV-MT接続強制解放応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTFORRELRES	cpridat_svmtforrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_SVMTFORRELRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT data sending   ) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(メーカMTデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_MKMTDATSND	cpridat_mkmtdatsnd;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTDATSND ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT connection request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(メーカMT接続要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_MKMTCNTREQ	cpridat_mkmtcntreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTCNTREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT connection response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(メーカMT接続応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
	CMT_CPRIF_MKMTCNTRES	cpridat_mkmtcntres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTCNTRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT release request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(メーカMT接続解放要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTCNTRELREQ	cpridat_mkmtcntrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTCNTRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT release response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */
 
/* CPRI信号受信通知(メーカMT接続解放応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTCNTRELRES	cpridat_mkmtcntrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTCNTRELRES ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT force release request   ) \n
 * 			Event number: 0x0301 \n
 * 
 */

/* CPRI信号受信通知(メーカMT接続強制解放要求)								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTFORRELREQ	cpridat_mkmtforrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTFORRELREQ ;

/**
 * @brief	CPRI Signal Receiving Notice   
 * @note	MESSAGE		: CPRI Signal Receiving Notice    \n
 * 						: (Maker MT force release response   ) \n
 * 			Event number: 0x0301 \n
 * 
 */


/* CPRI信号受信通知(メーカMT接続強制解放応答)								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTFORRELRES	cpridat_mkmtforrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_MKMTFORRELRES ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 			Event number: 0x0303 \n
 * 
 */


/*************************************************************************** */
/* MESSAGE :CPRI信号送信通知(CMT_TSKIF_CPRISNDNTC)							 */
/* 起動要因:0x0303															 */
/*************************************************************************** */
/* CPRI信号送信情報部  */
typedef struct
{
	USHORT						link_num;		/**< CPRI番号					 */
	UCHAR						dummy1;			/**< ダミー領域	1			 */
	UCHAR						dummy2;			/**< ダミー領域	2			 */
	UINT					i_link;				/**< I queue pointer			 */
	UINT					length;				/**< buffer length			 */
	UINT					disp;				/**< buffer offset sizes		 */
	UCHAR					Padding1;			/**< Padding1					 */
	UCHAR					Padding2;			/**< Padding2					 */
	UCHAR					Address;			/**< HDLC frame address area	 */
	UCHAR					Control;			/**< HDLC frame control area	 */
}CMT_CPRISIGSND_INF;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Signal kind getting   ) \n
 * 			Event number: 0x0303 \n
 * 
 */


/* CPRI信号送信通知(信号種別取得用)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	USHORT					signal_kind;		/**< 信号種別					 */
	USHORT					dummy;				/**< dummy領域				 */
} CMT_TSKIF_CPRISND_SIGNALGET;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (health check request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ヘルスチェック要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_HCREQ			cpridat_hcreq;		/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_HCREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (health check response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
 
/* CPRI信号送信通知(ヘルスチェック応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_HCRES			cpridat_hcres;		/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_HCRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE reset request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REリセット要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_RERSTREQ		cpridat_rerstreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RERSTREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE time set indication   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE時刻設定要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RETIMSETREQ	cpridat_retimsetreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RETIMSETREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE common link establish request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE共通部リンク確立要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_LINKSTSREQ	cpridat_linkstsreq; /**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_LINKSTSREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE common link establish response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE共通部リンク確立応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_LINKSTSRES	cpridat_linkstsres; /**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_LINKSTSRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (firmware information report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファームウェアファイル報告要求)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_FIRMFILEREQ	cpridat_firmfilereq;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FIRMFILEREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (firmware information report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファームウェアファイル報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_FIRMFILERES	cpridat_firmfileres;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FIRMFILERES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (firmware information report response   ) \n
 * 			Event number: 0x0303 \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/* CPRI信号送信通知(ファームウェアファイル報告応答2)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_FIRMFILERES2	cpridat_firmfileres2;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FIRMFILERES2;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file information report notification   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファイル情報報告通知)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_FILEINFREQ	cpridat_fileinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FILEINFREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file information report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファイル情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_FILEINFRES	cpridat_fileinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FILEINFRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file data sending   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファイルデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_FILEDATSND	cpridat_filedatsnd;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FILEDATSND;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file data sending complete report notice   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファイルデータ送信完了報告通知)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FILSNDFINNTC	cpridat_filsndfinntc;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FILSNDFINNTC;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file data sending complete report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(ファイルデータ送信完了報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FILSNDFINRES	cpridat_filsndfinres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FILSNDFINRES;

/* CPRI信号送信通知for S3G(ファイルデータ送信完了報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FILSNDFINRES_S3G	cpridat_filsndfinres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FILSNDFINRES_S3G;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (firmware information report request in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファームウェアファイル報告要求)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_UFIRMFILEREQ	cpridat_ufirmfilereq;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFIRMFILEREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (firmware information report response in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファームウェアファイル報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_UFIRMFILERES	cpridat_ufirmfileres;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFIRMFILERES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (firmware information report response in use state) \n
 * 			Event number: 0x0303 \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/* CPRI信号送信通知(運用中ファームウェアファイル報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_UFIRMFILERES2	cpridat_ufirmfileres2;/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFIRMFILERES2;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file information report notification in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファイル情報報告通知)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_UFILEINFREQ	cpridat_ufileinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFILEINFREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file information report response in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファイル情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_UFILEINFRES	cpridat_ufileinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFILEINFRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file data sending in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファイルデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_UFILEDATSND	cpridat_ufiledatsnd;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFILEDATSND;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file data sending complete report notice in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファイルデータ送信完了報告通知)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_UFILSNDFINNTC	cpridat_ufilsndfinntc;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFILSNDFINNTC;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (file data sending complete report response in use state) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(運用中ファイルデータ送信完了報告応答)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_UFILSNDFINRES	cpridat_ufilsndfinres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_UFILSNDFINRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (TRX set request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(TRX設定要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_TRXSETREQ		cpridat_trxsetreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_TRXSETREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (TRX set response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(TRX設定応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_TRXSETRES		cpridat_trxsetres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_TRXSETRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (TRX release request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(TRX解放要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_TRXRELREQ		cpridat_trxrelreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_TRXRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (TRX release response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(TRX解放応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_TRXRELRES		cpridat_trxrelres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_TRXRELRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Toffset report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(Toffset値報告要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_TOFFSETINFREQ	cpridat_toffsetinfreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_TOFFSETINFREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Toffset report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(Toffset値報告応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_TOFFSETINFRES	cpridat_toffsetinfres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_TOFFSETINFRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (no modulation carrier On/OFF specify request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(無変調キャリア出力ON/OFF指定要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFREQ	cpridat_caronoffreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARONOFFREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (no modulation carrier On/OFF specify response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(無変調キャリア出力ON/OFF指定応答 -3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFRES	cpridat_caronoffres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARONOFFRES;

/* CPRI信号送信通知(無変調キャリア出力ON/OFF指定応答 -S3G)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_CARONOFFRES_S3G	cpridat_caronoffres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARONOFFRES_S3G;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE equipment composition information report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE装置構成情報報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_EQPSTRREQ		cpridat_eqpstrreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EQPSTRREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE equipment composition information report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
 
/* CPRI信号送信通知(RE装置構成情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_EQPSTRRES		cpridat_eqpstrres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EQPSTRRES;

/* CPRI信号送信通知(RE装置構成情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_EQPSTRRES_S3G		cpridat_eqpstrres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EQPSTRRES_S3G;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE slot information report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REスロット情報報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_SLOTINFREQ	cpridat_slotinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SLOTINFREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE slot information report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REスロット情報報告応答)									 */
/* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			 */
/*	 に本フレームの使用はできない。											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_SLOTINFRES	cpridat_slotinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SLOTINFRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE status report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE状態報告要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_RESTSREQ		cpridat_restsreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RESTSREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE status report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE状態報告応答)											 */
/* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			 */
/*	 に本フレームの使用はできない。											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_RESTSRES		cpridat_restsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RESTSRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card information report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード情報報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARDINFREQ	cpridat_cardinfreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARDINFREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card information report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード情報報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARDINFRES	cpridat_cardinfres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARDINFRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE version report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REバージョン報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_REVERREQ		cpridat_reverreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REVERREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE version report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REバージョン報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_REVERRES		cpridat_reverres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REVERRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card control request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード制御要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECARDCNTREQ	cpridat_recardcntreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RECARDCNTREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card control response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード制御応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECARDCNTRES	cpridat_recardcntres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RECARDCNTRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card status report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード状態報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARDSTSREQ	cpridat_cardstsreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARDSTSREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card status report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード状態報告応答)		for 3G							 */
/*	※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			 */
/*	 に本フレームの使用はできない。											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARDSTSRES	cpridat_cardstsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARDSTSRES;


/* CPRI信号送信通知(REカード状態報告応答)	 for S3G								 */
/*	※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			 */
/*	 に本フレームの使用はできない。											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARDSTSRES_S3G	cpridat_cardstsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARDSTSRES_S3G;


/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE carrier status report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REキャリア状態報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARLSTSREQ	cpridat_carstsreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARLSTSREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE carrier status report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REキャリア状態報告応答)									 */
/* ※CPRI信号データ部は可変長となるためバッファ取得時のサイズ算出			 */
/*	 に本フレームの使用はできない。											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_CARLSTSRES	cpridat_carstsres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_CARLSTSRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE port control request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE PORT制御要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTCNTREQ	cpridat_reportcntreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REPORTCNTREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE port control response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE PORT制御応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTCNTRES	cpridat_reportcntres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REPORTCNTRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE port status report request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE PORT状態報告要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTSTSREQ	cpridat_reportstsreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REPORTSTSREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE port status report response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE PORT状態報告応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REPORTSTSRES	cpridat_reportstsres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REPORTSTSRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card diagnosis request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード診断要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECRDDIAREQ	cpridat_recrddiareq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RECRDDIAREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE card diagnosis response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(REカード診断応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_RECRDDIARES	cpridat_recrddiares;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_RECRDDIARES;

/************************************************************/
/* cascadeRRE 子RE->親RE応答メッセージ(PL->L3LOG間用) start */
/************************************************************/
/*************************************************************************** */
/* MESSAGE ：子REカード診断応答(CMD_TSKIF_CSCD_DIA_REQ)						 */
/* 信号種別：0x0E06		方向:PL->L3											 */
/*************************************************************************** */
/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice trans  \n
 * 						: (子REカード診断応答) \n
 * 			Event number: 0xC09 \n
 * 
 */
/* 子REカード診断応答(CMD_TSKIF_CSCD_DIA_REQ)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部			 */
	CMT_CPRIF_RECRDDIARES	cddiarcv;			/**< CPRI信号データ(PL->L3_LOG)	 */
}__attribute__ ((packed)) CMT_TSKIF_PLSND_CDDIARCV;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE trouble log get request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE障害ログ取得要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBSLOGREQ	cpridat_reobslogreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REOBSLOGREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE trouble log get response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE障害ログ取得応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBSLOGRES	cpridat_reobslogres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REOBSLOGRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE trouble log get stop request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE障害ログ取得中止要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号受信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBLOGSTPREQ	cpridat_reoblogstpreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REOBLOGSTPREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (RE trouble log get stop response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(RE障害ログ取得中止応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_REOBLOGSTPRES	cpridat_reoblogstpres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_REOBLOGSTPRES;


/************************************************************/
/* cascadeRRE 子RE->親RE応答メッセージ(PL->L3LOG間用) start */
/************************************************************/
/*************************************************************************** */
/* MESSAGE ：子RE障害ログ取得応答(CMD_TSKIF_CDTLOGGETREQ)					 */
/* 信号種別：0x0E06		方向:PL->L3											 */
/*************************************************************************** */
/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice trans  \n
 * 						: (子RE障害ログ取得応答) \n
 * 			Event number: 0x0E06 \n
 * 
 */
/* 子RE障害ログ取得応答(CMD_TSKIF_CDTLOGGETREQ)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部			 */
	CMT_CPRIF_REOBSLOGRES	cdtloggetreq;		/**< CPRI信号データ(PL->L3_LOG)	 */
}__attribute__ ((packed)) CMT_TSKIF_PLSND_CDTLOGGETREQ;

/*************************************************************************** */
/* MESSAGE ：子RE障害ログ受信通知(CMD_TSKIF_CDTLOGRCVTRAP)					 */
/* 信号種別：0x0E07			方向:PL->L3										 */
/*************************************************************************** */
/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice trans  \n
 * 						: (子RE障害ログ受信通知) \n
 * 			Event number: 0x0E07 \n
 * 
 */
/* 子RE障害ログ受信通知(CMD_TSKIF_CDTLOGRCVTRAP)								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部			 */
	CMT_CPRIF_UFILEDATSND	cdtlogrcvtrap;		/**< CPRI信号データ(PL->L3_LOG)	 */
}__attribute__ ((packed)) CMT_TSKIF_PLSND_CDTLOGRCVTRAP;

/*************************************************************************** */
/* MESSAGE ：子RE障害ログ受信完了通知(CMD_TSKIF_CDTLOGRCVCMPTRAP)			 */
/* 信号種別：0x0E08			方向:PL->L3										 */
/*************************************************************************** */
/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice trans  \n
 * 						: (子RE障害ログ受信完了通知) \n
 * 			Event number: 0x0E08 \n
 * 
 */
/* 子RE障害ログ受信完了通知(CMD_TSKIF_CDTLOGRCVCMPTRAP)							 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部			 */
	CMT_CPRIF_UFILSNDFINNTC	cdtlogrcvcmptrap;	/**< CPRI信号データ(PL->L3_LOG)	 */
}__attribute__ ((packed)) CMT_TSKIF_PLSND_CDTLOGRCVCMPTRAP;

/*************************************************************************** */
/* MESSAGE ：子RE障害ログ中断応答(CMD_TSKIF_CDTLOGSTOPRES)				 */
/* 信号種別：0x0E09			方向:PL->L3										 */
/*************************************************************************** */
/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice trans  \n
 * 						: (子RE障害ログ中断応答) \n
 * 			Event number: 0x0E09 \n
 * 
 */
/* 子RE障害ログ中断応答(CMD_TSKIF_CDTLOGSTOPRES)								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部			 */
	CMT_CPRIF_REOBLOGSTPRES	cdtlogstopres;		/**< CPRI信号データ(PL->L3_LOG)	 */
}__attribute__ ((packed)) CMT_TSKIF_PLSND_CDTLOGSTOPRES;
/************************************************************/
/* cascadeRRE 子RE->親RE応答メッセージ(PL->L3LOG間用) end   */
/************************************************************/


/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (external equipment data sending request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
/* CPRI信号送信通知(外部装置データ送信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATSNDREQ	cpridat_extdatsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EXTDATSNDREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (external equipment data sending response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(外部装置データ送信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATSNDRES	cpridat_extdatsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EXTDATSNDRES;


/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (external equipment data receiving request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(外部装置データ受信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATRCVREQ	cpridat_extdatrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EXTDATRCVREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (external equipment data receiving response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(外部装置データ受信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_EXTDATRCVRES	cpridat_extdatrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_EXTDATRCVRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (MT address set request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(MTアドレス設定要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MTADDSETREQ	cpridat_mtaddsetreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MTADDSETREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT connect request   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MT接続要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDMTCNCREQ	cpridat_fldmtcncreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDMTCNCREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT connect response   ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MT接続応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDMTCNCRES	cpridat_fldmtcncres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDMTCNCRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT command data sending request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータコマンド転送送信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMSNDREQ	cpridat_fldcomsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDCOMSNDREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT command data sending response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータコマンド転送送信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMSNDRES	cpridat_fldcomsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDCOMSNDRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT command data receiving request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータコマンド転送受信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMRCVREQ	cpridat_fldcomrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDCOMRCVREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT command data receiving response    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータコマンド転送受信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDCOMRCVRES	cpridat_fldcomrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDCOMRCVRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT file data sending request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータファイル転送送信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILSNDREQ	cpridat_fldfilsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDFILSNDREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT file data sending response    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータファイル転送送信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILSNDRES	cpridat_fldfilsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDFILSNDRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT file data receiving request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータファイル転送受信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILRCVREQ	cpridat_fldfilrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDFILRCVREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT file data receiving request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータファイル転送受信要求)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILRCVREQ_S3G	cpridat_fldfilrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDFILRCVREQ_S3G;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT file data receiving response    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MTデータファイル転送受信応答)						 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLDFILRCVRES	cpridat_fldfilrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLDFILRCVRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT release request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MT接続解放要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCNCRELREQ	cpridat_flmtcncrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLMTCNCRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT release response    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MT接続解放応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCNCRELRES	cpridat_flmtcncrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLMTCNCRELRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT force release request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MT接続強制解放要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCMPRELREQ	cpridat_flmtcmprelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLMTCMPRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (FLD-MT force release response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(FLD-MT接続強制解放応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FLMTCMPRELRES	cpridat_flmtcmprelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_FLMTCMPRELRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT connect request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MT接続要求)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_SVMTCNTREQ	cpridat_svmtcntreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTCNTREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT connect response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MT接続応答)											 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_SVMTCNTRES	cpridat_svmtcntres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTCNTRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT data sending request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MTデータ送信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATSNDREQ	cpridat_svmtdatsndreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTDATSNDREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT data sending response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MTデータ送信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATSNDRES	cpridat_svmtdatsndres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTDATSNDRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT data receiving request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MTデータ受信要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATRCVREQ	cpridat_svmtdatrcvreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTDATRCVREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT data receiving response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MTデータ受信応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTDATRCVRES	cpridat_svmtdatrcvres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTDATRCVRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT release request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MT接続解放要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTCNTRELREQ	cpridat_svmtcntrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTCNTRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT release response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MT接続解放応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTCNTRELRES	cpridat_svmtcntrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTCNTRELRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT force release request    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MT接続強制解放要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTFORRELREQ	cpridat_svmtforrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTFORRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (SV-MT force release response    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(SV-MT接続強制解放応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_SVMTFORRELRES	cpridat_svmtforrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_SVMTFORRELRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT data sending    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(メーカMTデータ送信)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_MKMTDATSND	cpridat_mkmtdatsnd;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTDATSND;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT connection request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(メーカMT接続要求)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_MKMTCNTREQ	cpridat_mkmtcntreq;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTCNTREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT connection response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(メーカMT接続応答)										 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
	CMT_CPRIF_MKMTCNTRES	cpridat_mkmtcntres;	/**< CPRI信号データ部			 */
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTCNTRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT release request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(メーカMT接続解放要求)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTCNTRELREQ	cpridat_mkmtcntrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTCNTRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT force release response     ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(メーカMT接続解放応答)									 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTCNTRELRES	cpridat_mkmtcntrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTCNTRELRES;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT force release request     ) \n
 * 			Event number: 0x0303 \n
 * 
 */


/* CPRI信号送信通知(メーカMT接続強制解放要求)								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTFORRELREQ	cpridat_mkmtforrelreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTFORRELREQ;

/**
 * @brief	CPRI Signal Sending Notice   
 * @note	MESSAGE		: CPRI Signal Sending Notice    \n
 * 						: (Maker MT force release response    ) \n
 * 			Event number: 0x0303 \n
 * 
 */
 
/* CPRI信号送信通知(メーカMT接続強制解放応答)								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_MKMTFORRELRES	cpridat_mkmtforrelres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_MKMTFORRELRES;

/**
 * @brief	L1 Data Sending Notice      
 * @note	MESSAGE		: L1 Data Sending Notice       \n
 * 			Event number: 0x0305 \n
 * 
 */
/* CPRI信号送信通知()								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FTPPORTSETREQ	cpridat_portsetreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_PORTSETREQ;

/**
 * @brief	L1 Data Sending Notice      
 * @note	MESSAGE		: L1 Data Sending Notice       \n
 * 			Event number: 0x0305 \n
 * 
 */
/* CPRI信号送信通知()								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FTPPORTSETRES	cpridat_portsetres;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_PORTSETRES;

 /**
 * @brief	RE Forwarding Notice
 * @note	MESSAGE		: RE Forwarding Notice     \n
 * 			Event number: 0x4001 \n
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
/* CPRI信号送信通知()								 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		 		/**< タスク共通ヘッダ			 */
	CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部		 */
												/**< CPRI信号データ部			 */
	CMT_CPRIF_FTPADDSETTOREQ	cpridat_addsettoreq;
}__attribute__ ((packed)) CMT_TSKIF_CPRISND_ADDSETTOREQ ;

/*************************************************************************** */
/* MESSAGE :L1データ送信要求(CMT_TSKIF_L1DATRQ)								 */
/* 起動要因:0x0305															 */
/*************************************************************************** */
typedef struct CMT_TSKIF_L1DATRQ
{
	CMT_TSKIF_HEAD				head;
	USHORT						link_num;		/**< CPRI番号					 */
	UCHAR						dummy1;			/**< ダミー領域	1			 */
	UCHAR						dummy2;			/**< ダミー領域	2			 */
// 擬似環境 20210310
//	struct CMT_TSKIF_L1DATRQ	*i_link;		/**< I queue pointer			 */
	UINT						i_link;		/**< I queue pointer			 */
// 擬似環境 20210310
	UINT						length;			/**< buffer length			 */
	UINT						disp;			/**< buffer offset sizes		 */
	UCHAR						Padding1;		/**< Padding1					 */
	UCHAR						Padding2;		/**< Padding2					 */
	UCHAR						Address;		/**< HDLC frame address area	 */
	UCHAR						Control;		/**< HDLC frame control area	 */
}CMT_TSKIF_L1DATRQ;

/**
 * @brief	Data Link Establishment confirm      
 * @note	MESSAGE		: Data Link Establishment confirm       \n
 * 			Event number: 0x0307 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :データリンク設定確認(CMT_TSKIF_L2ESTCNF)						 */
/* 起動要因:0x0307															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 	head;
	USHORT			link_num;
	UCHAR			dummy1;
	UCHAR			dummy2;
}CMT_TSKIF_L2ESTCNF;

/**
 * @brief	Data Link Establishment Indication      
 * @note	MESSAGE		: Data Link Establishment Indication       \n
 * 			Event number: 0x0309 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :データリンク設定表示(CMT_TSKIF_L2ESTIND)						 */
/* 起動要因:0x0309															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 	head;
	USHORT			link_num;
	UCHAR			dummy1;
	UCHAR			dummy2;
}CMT_TSKIF_L2ESTIND;

/**
 * @brief	Data Link Establishmenting indication      
 * @note	MESSAGE		: Data Link Establishmenting indication       \n
 * 			Event number: 0x030B \n
 * 
 */


/*************************************************************************** */
/* MESSAGE :データリンク設定中表示(CMT_TSKIF_L2ESTINGIND)					 */
/* 起動要因:0x030B															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 	head;
	USHORT			link_num;
	UCHAR			dummy1;
	UCHAR			dummy2;
}CMT_TSKIF_L2ESTINGIND;



/*************************************************************************** */
/* MESSAGE :L1データ受信通知(CMT_TSKIF_L1DATRCVNTC)							 */
/* 起動要因:0x0401															 */
/*************************************************************************** */
typedef struct CMT_TSKIF_L1DATRCVNTC
{
	CMT_TSKIF_HEAD					head;
	USHORT							link_num;	/**< CPRI番号					 */
	UCHAR							dummy1;		/**< ダミー領域	1			 */
	UCHAR							dummy2;		/**< ダミー領域	2			 */
// 擬似環境 20210310
//	struct CMT_TSKIF_L1DATRCVNTC	*i_link;	/**< I queue pointer			 */
	UINT							i_link;		/**< I queue pointer			 */
// 擬似環境 20210310
	UINT							length;		/**< buffer length			 */
	UINT							disp;		/**< buffer offset sizes		 */
	UCHAR							Padding1;	/**< Padding1					 */
	UCHAR							Padding2;	/**< Padding2					 */
	UCHAR							Address;	/**< HDLC frame address area	 */
	UCHAR							Control;	/**< HDLC frame control area	 */
}CMT_TSKIF_L1DATRCVNTC;

/**
 * @brief   L1データ受信通知(L2)
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct CMT_TSKIF_L1DATRCVNTC_L2
{
	CMT_TSKIF_HEAD					head;
	USHORT							link_num;	/**< CPRI番号					 */
	UCHAR							dummy1;		/**< ダミー領域	1			 */
	UCHAR							dummy2;		/**< ダミー領域	2			 */
// 擬似環境 20210310
//	struct CMT_TSKIF_L1DATRCVNTC	*i_link;	/**< I queue pointer			 */
	UINT							i_link;	/**< I queue pointer			 */
// 擬似環境 20210310
	UINT							length;		/**< buffer length			 */
	UINT							disp;		/**< buffer offset sizes		 */
	UCHAR							Padding1;	/**< Padding1					 */
	UCHAR							Padding2;	/**< Padding2					 */
	UCHAR							Address;	/**< HDLC frame address area	 */
	UCHAR							Control;	/**< HDLC frame control area	 */
	UINT							linkNum;
	UINT							allinfo;
	UINT							recvInfo[D_RRH_CPRINO_NUM][2];
}CMT_TSKIF_L1DATRCVNTC_L2;

/**
 * @brief	L1 Data Sending Complete Confirm      
 * @note	MESSAGE		: L1 Data Sending Complete Confirm       \n
 * 			Event number: 0x0402 \n
 * 
 */
 
 
/*************************************************************************** */
/* MESSAGE :Layer 1 Data CNF(CMT_TSKIF_L1DATCNF)							 */
/* 起動要因:0x0402															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 head;
	USHORT			link_num;		/**< CPRI番号					 */
	UCHAR			dummy1;			/**< ダミー領域	1			 */
	UCHAR			dummy2;			/**< ダミー領域	2			 */
}CMT_TSKIF_L1DATCNF;

/**
 * @brief	TX Sending Abnormal Notice      
 * @note	MESSAGE		: TX Sending Abnormal Notice       \n
 * 			Event number: 0x0501 \n
 * 
 */


/*************************************************************************** */
/* MESSAGE :TX送信異常通知(CMT_TSKIF_TXABNORMAL)							 */
/* 起動要因:0x0501															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;
	USHORT  			linkno;
	USHORT				dummy;
}CMT_TSKIF_TXABNORMAL;

/**
 * @brief	L1 Data Sending for Monitor Notice                     
 * @note	MESSAGE		: L1 Data Sending for Monitor Notice                     \n
 * 			Event number: 0x0503 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L1データ送信要求(Monitor)(CMT_TSKIF_L1MDATSNDREQ)				 */
/* 起動要因:0x0503															 */
/*************************************************************************** */
typedef struct CMT_TSKIF_L1MDATSNDREQ
{
	CMT_TSKIF_HEAD			head;
	USHORT						link_num;		/**< CPRI番号					 */
	UCHAR						dummy1;			/**< ダミー領域	1			 */
	UCHAR						dummy2;			/**< ダミー領域	2			 */
// 擬似環境 20210310
//	struct CMT_TSKIF_L1MDATSNDREQ    *i_link;	/**< I queue pointer			 */
	UINT							i_link;	/**< I queue pointer			 */
// 擬似環境 20210310
	UINT				length;					/**< buffer length			 */
	UINT				disp;					/**< buffer offset sizes		 */
	UCHAR				Padding1;				/**< Padding1					 */
	UCHAR				Padding2;				/**< Padding2					 */
	UCHAR				Address;				/**< HDLC frame address area	 */
	UCHAR				Control;				/**< HDLC frame control area	 */
}CMT_TSKIF_L1MDATSNDREQ;	



/**
 * @brief	L1 Data Receiving for Monitor Notice                    
 * @note	MESSAGE		: L1 Data Receiving for Monitor Notice                    \n
 * 			Event number: 0x0601 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L1データ受信通知(Monitor)(CMT_TSKIF_L1MDATRCVNTC)				 */
/* 起動要因:0x0601															 */
/*************************************************************************** */
typedef struct CMT_TSKIF_L1MDATRCVNTC
{
	CMT_TSKIF_HEAD				head;
	USHORT						link_num;		/**< CPRI番号					 */
	UCHAR						dummy1;			/**< ダミー領域	1			 */
	UCHAR						dummy2;			/**< ダミー領域	2			 */
// 擬似環境 20210310
//	struct CMT_TSKIF_L1MDATRCVNTC	*i_link;	/**< I queue pointer			 */
	UINT							i_link;	/**< I queue pointer			 */
// 擬似環境 20210310
	UINT						length;			/**< buffer length			 */
	UINT						disp;			/**< buffer offset sizes		 */
	UCHAR						Padding1;		/**< Padding1					 */
	UCHAR						Padding2;		/**< Padding2					 */
	UCHAR						Address;		/**< HDLC frame address area	 */
	UCHAR						Control;		/**< HDLC frame control area	 */
}CMT_TSKIF_L1MDATRCVNTC;


/**
 * @brief	TCP Connection Notice              
 * @note	MESSAGE		: TCP Connection Notice              \n
 * 			Event number: 0x0801 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :TCP接続通知(CMT_TSKIF_TCPCONNNTC)								 */
/* 起動要因:0x0801															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;    		    	/**< タスク共通ヘッダ			 */
	USHORT				linktype;		/**< TCP link type */
	USHORT				signal_kind;		/**< Signal type 3G/S3G	 */
	INT					linkhandle;		/**< TCP link handle  */
	INT					hostaddr;		/**< Related address */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	T_CMD_TCPCONN_NUM       sim_mon_no;     /**< sim/mon number              */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
}CMT_TSKIF_TCPCONNNTC;

/**
 * @brief	TCP Release Notice               
 * @note	MESSAGE		: TCP Release Notice               \n
 * 			Event number: 0x0803 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :TCP解放通知(CMT_TSKIF_TCPRELNTC)								 */
/* 起動要因:0x0803															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;    	   			/**< タスク共通ヘッダ			 */
	INT					linkhandle;				/**< TCP link handle			 */
	USHORT				signal_kind;			/**< signal kind 			*/
	USHORT				dummy;				/**< dummy				*/		
}CMT_TSKIF_TCPRELNTC;

/*************************************************************************** */
/* MESSAGE :TCP データ受信通知(CMT_TSKIF_TCPDATNTC)							 */
/* 起動要因:0x0805															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;   			/**< タスク共通ヘッダ				 */
	INT					linkhandle;			/**< TCP link handle				 */
	USHORT				signal_kind;			/**< signal kind 			*/
	USHORT				dummy;				/**< dummy				*/		
	UINT				msglen;				/**< 送信メッセージの長さ			 */
	UINT				msgevent;			/**< 送信メッセージの信号			 */
	UCHAR				msgbody[4];			/**< 送信メッセージのデータ		 */
}CMT_TSKIF_TCPDATNTC;

/**
 * @brief	TCP Connection Indication              
 * @note	MESSAGE		: TCP Connection Notice              \n
 * 			Event number: 0x0801 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :TCP接続通知(CMT_TSKIF_TCPCONNIND)								 */
/* 起動要因:0x0806															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;    		    	/**< タスク共通ヘッダ			 */
	UINT				hostaddr;			/**< server address */
	USHORT				port;					/**< server port	 */
	USHORT				dummy;				/**< dummy  */
}CMT_TSKIF_TCPCONNIND;


/* ========================================================================  */


/**
 * @brief	CPRI-MT Start Notice             
 * @note	MESSAGE		: CPRI-MT Start Notice             \n
 * 			Event number: 0x0901 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :メーカMT運用開始通知(CMT_TSKIF_MKMTSTRREQ,						 */
/* 								 CMT_TSKIF_CPRIMTSTRREQ)					 */
/* 起動要因:0x0901															 */
/*************************************************************************** */
/* メーカMT管理 --> 通信管理		 */
typedef struct
{
	CMT_TSKIF_HEAD	head;					/**< タスク共通ヘッダ				 */
	USHORT			mode;					/**< Mode(0:monitor/1:simulator)	 */
	USHORT			dummy;					/**< Dummy						 */
}CMT_TSKIF_MKMTSTRREQ;

/* メーカMT管理 --> TXIOCS,RXIOCS	 */
typedef struct
{
	CMT_TSKIF_HEAD		head;				/**< タスク共通ヘッダ				 */
	USHORT				mode;				/**< Mode(0:monitor; 1:simulator)	 */
	USHORT				dummy;				/**< Dummy						 */
	USHORT 				linkflg[CMD_LINK_NUM_M];/**< Link flag(1:care;0:not care) */
}CMT_TSKIF_CPRIMTSTRREQ;

/**
 * @brief	CPRI-MT Stop Notice              
 * @note	MESSAGE		: CPRI-MT Stop Notice              \n
 * 			Event number: 0x0903 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :メーカMT運用停止通知(CMT_TSKIF_MKMTSTPREQ,						 */
/*								 CMT_TSKIF_CPRIMTSTPREQ)					 */
/* 起動要因:0x0903															 */
/*************************************************************************** */
/* メーカMT管理 --> 通信管理		 */
typedef struct
{
	CMT_TSKIF_HEAD	head;					/**< タスク共通ヘッダ				 */
	USHORT			mode;					/**< Mode(0:monitor/1:simulator)	 */
	USHORT			dummy;					/**< Dummy						 */
}CMT_TSKIF_MKMTSTPREQ;

/* メーカMT管理 --> TXIOCS,RXIOCS	 */
typedef struct
{
	CMT_TSKIF_HEAD	head;					/**< タスク共通ヘッダ				 */
	USHORT			mode;					/**< Mode(0:monitor/1:simulator)	 */
	USHORT			dummy;					/**< Dummy						 */
	USHORT 			linkflg[CMD_LINK_NUM_T]; /**< Link flag(1:care;0:not care) */
}CMT_TSKIF_CPRIMTSTPREQ;


/**
 * @brief	CPRI Simulator Start Request                
 * @note	MESSAGE		: CPRI Simulator Start Request                \n
 * 			Event number: 0x0905 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :CPRI Simulator開始要求(CMD_TSKIF_CPRISIMSTRREQ)					 */
/* 起動要因:0x0905															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				linkno;					/**< link番号					 */
	USHORT				simmode;				/**< CPRI simulator mode		 */
}CMT_TSKIF_CPRISIMSRTREQ;

/**
 * @brief	CPRI Simulator Start Response                 
 * @note	MESSAGE		: CPRI Simulator Start Response                 \n
 * 			Event number: 0x0906 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :CPRI Simulator開始応答(CMD_TSKIF_CPRISIMSTRRSP)					 */
/* 起動要因:0x0906															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				linkno;					/**< link番号					 */
	USHORT				result;					/**< 応答結果					 */
}CMT_TSKIF_CPRISIMSRTRSP;

/**
 * @brief	CPRI Simulator Stop Request                  
 * @note	MESSAGE		: CPRI Simulator Stop Request                  \n
 * 			Event number: 0x0907 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :CPRI Simulator停止要求(CMD_TSKIF_CPRISIMSTPREQ)					 */
/* 起動要因:0x0907															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;        			/**< タスク共通ヘッダ			 */
	USHORT				cpri_no_range;			/**< CPRI simulator link 
															specify range	 */
	USHORT				linkno;					/**< link番号				 */
}CMT_TSKIF_CPRISIMSTPREQ;


/**
 * @brief	CPRI Simulator Stop Response                   
 * @note	MESSAGE		: CPRI Simulator Stop Response                   \n
 * 			Event number: 0x0908 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :CPRI Simulator停止応答(CMD_TSKIF_CPRISIMSTPRSP)					 */
/* 起動要因:0x0908															 */
/*************************************************************************** */
typedef struct
{
	USHORT				linkno;					/**< link番号					 */
	USHORT				result;					/**< 応答結果					 */
}CMT_TSKIF_ONELINKRSP;	

typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	USHORT					linknum;			/**< total link number		 */
	CMT_TSKIF_ONELINKRSP	stopresult[12];		/**< 応答結果					 */
	USHORT					dummy;				/**< ダミー領域				 */
}CMT_TSKIF_CPRISIMSTPRSP;	



/*************************************************************************** */
/* MESSAGE :CPRI Monitor開始通知(CMD_TSKIF_CPRIMONSTRREQ)					 */
/* 起動要因:0x0909															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				cpri_no;				/**< CPRI 番号				 */
	USHORT				dummy;					/**< Dummy					 */
} CMT_TSKIF_CPRIMONSTRREQ;

/*************************************************************************** */
/* MESSAGE :CPRI Monitor停止通知(CMD_TSKIF_CPRIMONSTPREQ)					 */
/* 起動要因:0x090B															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				cpri_no;				/**< CPRI 番号				 */
	USHORT				dummy;					/**< Dummy					 */
} CMT_TSKIF_CPRIMONSTPREQ;




/* ========================================================================  */
/* MESSAGE :TCP データ送信要求(CMT_TSKIF_MTTCPSNDREQ)						 */
/* 起動要因:0x090D															 */
/* ========================================================================  */
/* ========================================================================  */
/* Direction : MTM -> MLS													 */
/* ========================================================================  */

typedef struct
{
	CMT_TSKIF_HEAD		head;        		/**< タスク共通ヘッダ				 */
	INT					linkhandle;			/**< TCP link handle				 */
	UINT				msglen;				/**< 送信メッセージの長さ			 */
	UCHAR				msgbody[4];			/**< 送信メッセージのデータ		 */
}CMT_TSKIF_MTTCPSNDREQ;

/*************************************************************************** */
/* RTC	structure For TCP データ送信要求									 */
/*************************************************************************** */
typedef struct
{
	INT 	year :		12;					/**< Year 領域					 */
	INT		month :		8;					/**< Month 領域					 */
	INT		day :		8;					/**< Day 領域						 */
	INT		hour :		8;					/**< Hour 領域					 */
	INT		min :		8;					/**< Minitus 領域					 */
	INT		sec :		8;					/**< Second 領域					 */
	INT		msec :		12;					/**< Millisecond 領域				 */
}CMT_MKRTC;
/*************************************************************************** */
/* Response	to MK MT client													 */
/*************************************************************************** */
typedef struct
{
	UINT	result;							/**< 応答結果						 */
	UINT	cause;							/**< 結果詳細						 */
}CMT_MKRSP;
	
/* ========================================================================  */
/* Direction : MKM -> LNS													 */
/* ========================================================================  */
typedef struct 
{
	VOID*			msgbody;			/**< 送信メッセージのデータ		 */
	INT				linkhandle;			/**< TCP link handle				 */
	UINT				msglen;				/**< 送信メッセージの長さ			 */
	UINT				msgevent;			/**< 送信メッセージのevent number	 */
// 擬似環境 20210313
	UCHAR			padding[CMD_NUM4];	/**< ポインタが8byteなのでpaddingが発生するので追加 */
// 擬似環境 20210313
	UINT				dummy1;
	UINT				dummy2;
}CMT_TSKIF_MKMTHEAD;

typedef struct
{
	CMT_TSKIF_HEAD head;	/**< タスク共通ヘッダ				 */
	CMT_TSKIF_MKMTHEAD mkmt_head;	/**< Maker MT header	 */
}CMT_TSKIF_MKMTMONDAT;	/**< Maker MT monitor data header 	 */

typedef struct
{
	CMT_TSKIF_HEAD		head;        		/**< タスク共通ヘッダ				 */
	VOID*				msgbody;			/**< 送信メッセージのデータ		 */
	INT					linkhandle;			/**< TCP link handle				 */
	UINT				msglen;				/**< 送信メッセージの長さ			 */
	UINT				msgevent;			/**< 送信メッセージのevent number	 */
	union{
		CMT_MKRTC		rtc;				/**< 送信メッセージのRTC			 */
		CMT_MKRSP		rsp;				/**< Response message 			 */
	}rtc_rsp;
	UINT				dummy;				/**< ダミー領域					 */
}CMT_TSKIF_TCPSNDREQ;

/*************************************************************************** */
/* MESSAGE :TCP データ送信応答(CMT_TSKIF_TCPSNDRSP)							 */
/* 起動要因:0x090E															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;        		/**< タスク共通ヘッダ				 */
	INT					linkhandle;			/**< TCP link handle				 */
	UINT				result;				/**< 応答結果						 */
}CMT_TSKIF_TCPSNDRSP;


/**
 * @brief	3G RF TRX setting Notice           
 * @note	MESSAGE		: 3G RF TRX setting Notice            \n
 * 			Event number: 0x0D01 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :3G RF TRX設定通知(CMT_TSKIF_3GRF_TRXSETNTC)					 */
/* 起動要因:0x0D01															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/*  タスク共通ヘッダ		*/
	USHORT				carrier_no[CMD_MAX_CARNUM];
	USHORT				freq_no[CMD_MAX_CARNUM];
} CMT_TSKIF_3GRF_TRXSETNTC;


/**
 * @brief	L3 Establishment Notice           
 * @note	MESSAGE		: L3 Establishment  Notice            \n
 * 			Event number: 0x0F01 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L3ヘルスチェック確立通知(CMT_TSKIF_L3ESTNTC)					 */
/* 起動要因:0x0F01															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			 */
	USHORT				cpri_no;				/**< CPRI番号					 */
	USHORT				system_type;		/**< system type */
}CMT_TSKIF_L3ESTNTC;			


/**
 * @brief	L3 Disconnection Notice            
 * @note	MESSAGE		: L3 Disconnection Notice             \n
 * 			Event number: 0x0F03 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :L3ヘルスチェック断通知(CMT_TSKIF_L3DISCNNTNTC)					 */
/* 起動要因:0x0F03															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			 */
	USHORT				cpri_no;				/**< CPRI番号					 */
	USHORT				system_type;		/**< system type  */

}CMT_TSKIF_L3DISCNNTNTC;

/* ========================================================================  */
/* MESSAGE :L2リンク断依頼通知(CMT_TSKIF_XXX)								 */
/* 起動要因:0x0F05															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */

/* ========================================================================  */
/* MESSAGE :RE共通部リンク再確立通知(CMT_TSKIF_LNKSETNTC)					 */
/* 起動要因:0x0F07															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**<  タスク共通ヘッダ		 */
	USHORT				set_kind;				/**< Active/Negative種別設定	 */
	USHORT				system_type;		/**< system type  */
	USHORT				set_cause;				/**< 切替要因設定	 */
	USHORT				dummy;					/**< dummy area */
} CMT_TSKIF_LNKSETNTC;


/* ========================================================================  */
/* MESSAGE :ローカルALM通知(CMT_TSKIF_XXX)								 */
/* 起動要因:0x0F09															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */

/* ========================================================================  */
/* MESSAGE :ローカルリセット通知(CMT_TSKIF_XXX)							 */
/* 起動要因:0x0F0B															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */

/* ========================================================================  */
/* MESSAGE :TRX設定通知(CMT_TSKIF_TRXSETNTC)								 */
/* 起動要因:0x0F0D															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**<  タスク共通ヘッダ		 */
	USHORT				stachg_br0;			/**< Br0状態変化			 */
	USHORT				stachg_br1;			/**< Br1状態変化			 */
	USHORT				system_type;			/**< system type */
	USHORT				dummy;					/**< dummy area */
} CMT_TSKIF_TRXSETNTC;

/* TDD SRE TRX setting S. */
/* ========================================================================  */
/* MESSAGE :TRX解放通知(CMT_TSKIF_TRXRLSNTC)								 */
/* 起動要因:0x0F0F															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**<  タスク共通ヘッダ		 */
	USHORT				carrno;					/**< Carrier No.*/
	USHORT				stachg_br0;				/**< Br0状態変化			 */
	USHORT				stachg_br1;				/**< Br1状態変化			 */
	USHORT				stachg_br2;				/**< Br2状態変化			 */
	USHORT				stachg_br3; 			/**< Br3状態変化			 */
	USHORT				system_type;			/**< system type */
} CMT_TSKIF_TRXRLSNTC;
/* TDD SRE TRX setting E. */

/* ========================================================================  */
/* MESSAGE :Local Block Control(CMT_TSKIF_LCKBLKCNT)								 */
/* 起動要因:0x0F0F															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**<  タスク共通ヘッダ		 */
	UINT				onoff_flg;				/**< ON/OFF Flag		 */
} CMT_TSKIF_LCKBLKCNTREQ;

/* ========================================================================  */
/* MESSAGE :Local Block Control(CMT_TSKIF_LCKBLKCNT)								 */
/* 起動要因:0x0F0F															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**<  タスク共通ヘッダ		 */
	UINT				result;					/**< Result				 */
} CMT_TSKIF_LCKBLKCNTRES;

/* ========================================================================  */
/* MESSAGE :TCP Server 初期化要求(CMT_TSKIF_TCPSRVINITREQ)				 */
/* 起動要因:0x0F15															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */
typedef struct
{
	CMT_TSKIF_HEAD		head;        	/* タスク共通ヘッダ					*/
	USHORT 				obj;			/* 対象MT							*/
#if 0
	UCHAR				macaddr[6];		/* MAC	アドレス					*/
	UCHAR 				ipaddr[4];		/* IPアドレス						*/
	UCHAR 				ipmask[4];		/* サブネットマスク					*/
#endif
	USHORT 				fmcmdport;		/* Port for FLD-MT data command transfer */
	USHORT 				fmfilport;		/* Port for FLD-MT data file transfer	*/
	USHORT 				smdatport;		/* Port for SV-MT data transfer		*/	
	USHORT 				simuport;		/* ポート番号 for メーカMT simulator*/
	USHORT 				monport;		/* ポート番号 for メーカMT monitor	*/
	USHORT 				remtport;		/* ポート番号 for RE MT			*/
	
}CMT_TSKIF_TCPSRVINITREQ;

/*************************************************************************** */
/* MESSAGE :TCP Server初期化要求(CMT_TSKIF_TCPSRVINITRSP)					 */
/* 起動要因:0x0F16															 */
/*************************************************************************** */
/* データ部情報なしのためフレーム定義しない  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/* タスク共通ヘッダ			*/
	USHORT				obj;					/* 対象MT					*/
	USHORT				result;					/* 応答結果					*/
}CMT_TSKIF_TCPSRVINITRSP;



/*************************************************************************** */
/* MESSAGE : スリープ制御通知(CMT_TSKIF_SLPCTRLNTC)							 */
/* 起動要因:0x1003															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ		 */
	USHORT				cnt_kind;				/**< 制御					 */
	USHORT				reserve;				/**< reserve				 */
} CMT_TSKIF_SLPCTRLNTC;


/* ========================================================================  */
/* MESSAGE :TCP Server起動要求(CMT_TSKIF_XXX)								 */
/* 起動要因:0x1201															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD	head;        	/**< タスク共通ヘッダ						 */
	UINT 			obj;			/* 対象MT							*/
	UCHAR			macaddr[6];		/* MAC	アドレス					*/
	UCHAR 			ipaddr[4];		/**< IPアドレス							 */
	UCHAR 			ipmask[4];		/**< サブネットマスク						 */
	USHORT 			fmcmdport[CMD_MAX_SYSNUM];		/**< ポート番号 for FLD-MTデータコマンド転送  */
	USHORT 			fmfilport[CMD_MAX_SYSNUM];		/**< ポート番号 for FLD-MTデータファイル転送  */
	USHORT 			smdatport[CMD_MAX_SYSNUM];		/**< ポート番号 for SV-MTデータ転送	for 3G	 */
	USHORT 			simuport;		/**< ポート番号 for CPRI-MT simulator */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT 			simuport_re1;		/**< ポート番号 for CPRI-MT simulator (RE1) */
	USHORT 			simuport_re2;		/**< ポート番号 for CPRI-MT simulator (RE2) */
	USHORT 			simuport_re3;		/**< ポート番号 for CPRI-MT simulator (RE3) */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT 			monport;		/**< ポート番号 for CPRI-MT monitor	 */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT 			monport_re1;		/**< ポート番号 for CPRI-MT monitor (RE1)	 */
	USHORT 			monport_re2;		/**< ポート番号 for CPRI-MT monitor (RE2)	 */
	USHORT 			monport_re3;		/**< ポート番号 for CPRI-MT monitor (RE3)	 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT 			remtport;		/**< ポート番号 for RE-MT			 */

}CMT_TSKIF_TCPSRVSTRREQ;
/* ========================================================================  */
/* MESSAGE :TCP Server起動応答(CMT_TSKIF_XXX)								 */
/* 起動要因:0x1202															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	UINT	 			obj;						/* 対象MT							*/
	INT					result;					/**< 応答結果					 */
}CMT_TSKIF_TCPSRVSTRRSP;


/* ========================================================================  */
/* MESSAGE :タイムアウト発生通知(CMT_TSKIF_TONTC)							 */
/* 起動要因:0x1601															 */
/* ========================================================================  */
/* データ部情報なしのためフレーム定義しない  */



/**
 * @brief	L2 Busy Notice     
 * @note	MESSAGE		: L2 Busy Notice      \n
 * 			Event number: 0x1603 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :自受信ビジー(CMT_TSKIF_L2BSY)									 */
/* 起動要因:0x1603															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 head;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT link_num;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT			buftyp;	
}CMT_TSKIF_L2BSY;

/**
 * @brief	L2 Busy Clear Notice     
 * @note	MESSAGE		: L2 Busy Clear Notice      \n
 * 			Event number: 0x1605 \n
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :自受信ビジーの解除(CMT_TSKIF_L2BSYCLR)							 */
/* 起動要因:0x1605															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD	 head;
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT link_num;
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT			buftyp;	
}CMT_TSKIF_L2BSYCLR;

/**
 * @brief	L1 Data Receiving Notice      
 * @note	MESSAGE		: L1 Data Receiving Notice       \n
 * 			Event number: 0x0401 \n
 * 
 */
 


/**
 * @brief	CPRI State Change Interrupt Notice         
 * @note	MESSAGE		: CPRI State Change Interrupt Notice          \n
 * 			Event number: 0x1609 \n
 * 
 */

/* ========================================================================  */
/* MESSAGE :CPRI State変化通知(CMT_TSKIF_CPRISTIRQNTC)						 */
/* 起動要因:0x1609															 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	UINT				data;				/**< IRQ割込情報				 */
} CMT_TSKIF_CPRISTIRQNTC;

/**
 * @brief	CPRI link parameter resetting request
 * @note	MESSAGE		: CPRI link parameter resetting request          \n
 * 			Event number: 0x1635 \n
 * 
 */

/* ========================================================================  */
/* MESSAGE :System Parameter Change Notification(CMT_TSKIF_SYSPARA_CHG_NTC)			 */
/* 起動要因:0x0D05															 */
/* ========================================================================  */
typedef struct
{
	USHORT				tx_sys_num_old;					/**<  old tx system number	*/
	USHORT				tx_sys_num_new;					/**<  new tx system number	*/
} CMT_TSKIF_SYSPARA_CHG_INF;

typedef struct
{
	CMT_TSKIF_HEAD				head;				/**<  タスク共通ヘッダ		 */
	CMT_TSKIF_SYSPARA_CHG_INF	inf;				/**<  System Parameter Chg Information	 */
} CMT_TSKIF_SYSPARA_CHG_NTC;

/* ======================================================================== */
/* Message common head between CPRI MT/RE-MT and TRA								*/
/* ======================================================================== */
typedef struct{
	UINT				msgid;					/**< message ID  				*/
	UINT				srcFbNo;				/**< source FB number  		    */
	UINT				dstFbNo;				/**< destination FB number  	*/
	UINT				allMsgLen;				/**< total message length  		*/
	UINT				replyId;				/**< reply Id  				    */
	UINT				headSeqNo;				/**< head sequence number  	    */
	USHORT				next;					/**< next flag  				*/
	USHORT				msgLen;					/**< single message length  	*/
	UINT				nowSeqNo;				/**< current sequence number	*/
}T_MSGHEAD;

/**
 * @brief   REC CPRI link preparation request
 * @note    起動要因:0x4101
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
} CMT_TSKIF_CPRILNKPRE_REQ;

/**
 * @brief   REC CPRI link preparation response
 * @note    起動要因:0x4102
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
} CMT_TSKIF_CPRILNKPRE_RES;

/**
 * @brief   REC CPRI link start notice
 * @note    起動要因:0x4103
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
} CMT_TSKIF_REC_CPRILNKSTART_NTC;

/**
 * @brief   RE CPRI link start notice
 * @note    起動要因:0x4105
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ				 */
	USHORT				link_num;		        /**< CPRI番号						 */
	UINT                sfpLogTrigger;          /**< SFPログ履歴取得契機             */
} CMT_TSKIF_RE_CPRILNKSTART_NTC;

/**
 * @brief   CPRI connection notice (Layer3 OK)
 * @note    起動要因:0x4107
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ				*/
	USHORT				link_num;				/**< CPRI番号				*/
	USHORT				system_type;			/**< lte or 3g				*/
} CMT_TSKIF_CPRICONN_NTC;

/**
 * @brief   CPRI disconnection notice
 * @note    起動要因:0x4109
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 	*/
	USHORT				link_num;				/**< CPRI番号				*/
	USHORT				layer_kind;				/**< 0:L1/L2断 1:L3断			*/
	USHORT				system_type;			/**< lte or 3g				*/
					/** layer_kind=1の時のみ有効 */
} CMT_TSKIF_CPRIDISCONN_NTC;

/**
 * @brief   RE CPRI link stop notice
 * @note    起動要因:0x410B
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
    CMT_TSKIF_HEAD      head;                   /**< タスク共通ヘッダ        */
    USHORT              link_num;               /**< CPRI番号                */
} CMT_TSKIF_RE_CPRILNKSTOP_NTC;

/**
 * @brief   RE立ち上げ要求
 * @note    起動要因:0x4005
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			*/
	USHORT				link_num;				/**< CPRI番号					*/
	USHORT				system_type;			/**< lte or 3g					*/
	UCHAR				dummy1;					/**< ダミー領域1 				*/
	UCHAR				dummy2;					/**< ダミー領域2 				*/
}CMT_TSKIF_RESTR_REQ;	
 	
/**
 * @brief	RE Disconnecion Request
 * @note	MESSAGE		: RE Disconnecion Request
 * 			Event number: 0x4007
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */

/*************************************************************************** */
/* MESSAGE :RE 断要求(CMT_TSKIF_REDSCREQ)									 */
/* 起動要因:0x4007															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ				*/
	USHORT				link_num;				/**< CPRI番号				*/
	USHORT				layer_kind;				/**< 0:L1/L2断 1:L3断			*/
	USHORT				system_type;			/**< lte or 3g				*/
					/** layer_kind=1の時のみ有効 */
}CMT_TSKIF_REDIS_REQ;	
 	
/**
 * @brief	RE Startup Response
 * @note	MESSAGE		: RE Startup Response 
 * 			Event number: 0x4006
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :RE 立ち上げ応答(CMT_TSKIF_RESTRRES)								 */
/* 起動要因:0x4006															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			*/
	USHORT				link_num;				/**< CPRI番号					*/
	USHORT				system_type;			/**< lte or 3g					*/
	UCHAR				dummy1;					/**< ダミー領域1 				*/
	UCHAR				dummy2;					/**< ダミー領域2 				*/
	USHORT				result;					/* 応答結果					*/
}CMT_TSKIF_RESTR_RSP;	
 	
/**
 * @brief	RE Disconnection Response
 * @note	MESSAGE		: RE Disconnection Response
 * 			Event number: 0x4008
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * 
 */
 
/*************************************************************************** */
/* MESSAGE :RE 断応答(CMT_TSKIF_REDSCRES)									 */
/* 起動要因:0x4008															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ				*/
	USHORT				link_num;				/**< CPRI番号				*/
	USHORT				layer_kind;				/**< 0:L1/L2断 1:L3断			*/
	USHORT				system_type;			/**< lte or 3g				*/
						/** layer_kind=1の時のみ有効 */
	USHORT				result;					/* 応答結果					*/
}CMT_TSKIF_REDIS_RSP;
 	
/**
 * @brief   RE強制停止通知
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				link_num;		/**< CPRI番号					 */
	USHORT              system_type;
	T_RRH_RESTOP_FACTOR stopFactor;
} CMT_TSKIF_RECMPSTPNTC;

/**
 * @brief   RE強制停止指示
 * @note    起動要因:0x4311
 * @note    direction:l3_rct->re_rec
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ		*/
	USHORT				link_num;				/**< CPRI番号				*/
	USHORT 				system_type;			/**< 0:3G 1:LTE				*/
	T_RRH_RESTOP_FACTOR stopFactor;             /**< 強制停止要因 */
} CMT_TSKIF_RE_FORCESTOP_IND;

/**
 * @brief   RE強制リセット指示
 * @note    起動要因:0x4312
 * @note    direction:l3_rct->re_rec
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ		*/
	USHORT				link_num;				/**< CPRI番号				*/
} CMT_TSKIF_RE_FORCERESET_IND;

/**
 * @brief   RE強制停止通知
 * @note    起動要因:0x4313
 * @note    direction:re_rec->l3_rct
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ		*/
	USHORT				link_num;				/**< CPRI番号		*/
	USHORT				system_type;			/**< lte or 3G		*/
	T_RRH_RESTOP_FACTOR stopFactor;             /**< 強制停止要因 */
} CMT_TSKIF_RE_FORCESTOP_NTC;


/**
 * @brief	cpri state change notice
 * @note	MESSAGE		:  cpri state change notice
 * 			Event number: 0x4621
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */

/*************************************************************************** */
/* MESSAGE :状態変化通知(CMD_TSKIF_CPRSTACHGNTC)							 */
/* 起動要因:0x4621															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			*/
	USHORT				link_num;				/**< CPRI番号					*/
	USHORT				cprista;				/**< 状態変化					*/


}CMT_TSKIF_CPRISTATECHG_NTC;


/**
 * @brief	cpri state change notice
 * @note	MESSAGE		:  cpri state change notice
 * 			Event number: 0x4621
 * @date    2020/12/28 M&C) Merge 4G FHM src
 *
 */

/*************************************************************************** */
/* MESSAGE :２次リセット要求(CMD_TSKIF_2NDRST_REQ)							 */
/* 起動要因:0x4623															 */
/*************************************************************************** */
typedef struct
{
	CMT_TSKIF_HEAD		head;		 			/**< タスク共通ヘッダ			*/
	USHORT				link_num;				/**< CPRI番号					*/


}CMT_TSKIF_2NDRST_REQ;

/*************************************************************************** */
/* 																			 */
/*	No.6	カード内共通テーブル構造体定義									 */
/* 																			 */
/*************************************************************************** */


/* ======================================================================== */
/* アラーム詳細情報  */
/* ======================================================================== */
typedef struct{
	USHORT				alarm[CMD_ALMFACTOR_NUM];	/**< TRAアラーム詳細要因情報(0-5) */
	USHORT				rf_alarm;					/**< RF共用部異常情報	*/
	USHORT				dummy;						/**< dummy   */
}CMT_TRAALMTBL;

/**
 * @brief	TRA SV state management table
 * @note	Contents:	TRA SV state management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */

//TD-SRE update
typedef struct{
	USHORT				en_flg;								/**< EN flag   */
	USHORT				sv_inf[8];				/**< TRA SV bus information   */
	USHORT				alm_num;							/**< occurred alarm number   */
	USHORT				alm_inf[255];			/**< occurred alarm information   */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT              err_num;
	USHORT              err_inf[255];
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT				card_cntitem;						/**< Card Control item			 */
	USHORT				card_extitem;						/**< Card Ext Control Item		*/
	USHORT				carr_onoff;							/**< Carrier ON/OFF   */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	struct {
	    USHORT          alm_num;
	    USHORT          alm_inf[255];
	    USHORT          err_num;
	    USHORT          err_inf[255];
	} sys[CMD_MAX_SYSNUM];
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
}CMT_TRASVTBL;

/**
 * @brief	external interface common management table
 * @note	Contents:external interface management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
  */

typedef struct {
	USHORT				port_in;							/**< PORT input indication  */
	USHORT				port_out;							/**< PORT output  */
	USHORT				extif_inf;							/**< external IF status information  */
	USHORT				shf_inf;							/**< shelf status information  */
	USHORT				alm_num;							/**< occurred alarm number  */
	USHORT				alm_inf[CMD_MAX_ALM_NUM];			/**< occurred alarm information  */
	USHORT				fanalm_flg;							/**< FAN ALM検出 flag */
} CMT_EXTIFTBL;

/*TDD SRE  TRX setting S. */
/**
 * @brief	[TRX setting]Carrier status management table
 * @note	Contents:	carrier setting status management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 */
typedef struct
{
	USHORT				car_onoff;							/**< carrier ON/OFF   */
	USHORT				card_car_onoff;						/**< card carrier ON/OFF for card state response: don't care/dummy  */
	USHORT				bandwidth;							/**< システム帯域幅	for S3G			 */
	USHORT				dl_freq;							/**< 下り周波数番  for S3G  */
	USHORT				snd_br;								/**< 送信系	for S3G */
	USHORT				dummy;								/**< dummy*/
}CMT_TRX_CAR_INF;

/**
 * @brief	TRX setting status management table
 * @note	Contents:	carrier setting status management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
  */
typedef struct{
	CMT_TRX_CAR_INF     trx_car_inf[CMD_MAX_CARNUM];		/**< carrier information   */
	USHORT				tdd_up_dwn_config;	/**< 下り周波数番号が3.5GHz帯の場合のみ有効とする．
								     		それ以外の下り周波数番号の場合は、Don’t careとする．*/
	USHORT				tdd_spe_sf_config;	/**< 下り周波数番号が3.5GHz帯の場合のみ有効とする．
								     		それ以外の下り周波数番号の場合は、Don’t careとする．*/
	USHORT				tdd_dwnlk_cp_len;	/**< 0：Normal cyclic prefix
								     			1：Extended cyclic prefix
								     			下り周波数番号が3.5GHz帯の場合のみ有効とする．
								     			それ以外の下り周波数番号の場合は、Don’t careとする．*/	
	USHORT				snrv_onoff;							/**< sending/receiving ON/OFF   */
	USHORT				nowav_state_br0;					/**< No wave state for BR#0   */
	USHORT				nowav_point_br0;					/**< No wave for BR#0 register access location   */
	USHORT				nowav_state_br1;					/**< No wave state for BR#1   */
	USHORT				nowav_point_br1;					/**< No wave for BR#1 register access location   */
	USHORT				nowav_state_br2;					/**< No wave state for BR#1   */
	USHORT				nowav_point_br2;					/**< No wave for BR#1 register access location   */
	USHORT				nowav_state_br3;					/**< No wave state for BR#1   */
	USHORT				nowav_point_br3;					/**< No wave for BR#1 register access location   */
	
}CMT_TRXSETTBL;
/*TDD SRE TRX setting  E. */

/**
 * @brief	カード制御ブランチのスリープ情報
 * @note	-
 * 
  */
typedef struct{
	USHORT					state;				/**< 状態									*/
}CMT_CARDCNTBRSLPINFO;

/**
 * @brief	カード制御ブランチのPA情報
 * @note	-
 * 
  */
typedef struct{
	USHORT					warmup_flg;			/**< Warm up flag							*/
	USHORT					pa_flg;				/**< PA-ON/OFF								*/
}CMT_CARDCNTBRPAINFO;

/**
 * @brief	カード制御のブランチ情報
 * @note	-
 * 
  */
typedef struct{
	USHORT					exc_code;			/**< 制御コード(3G)s						*/
	USHORT					exc_itm;			/**< 制御項目								*/
	CMT_CARDCNTBRSLPINFO	slpInfo;			/**< スリープ情報							*/
	CMT_CARDCNTBRPAINFO		paInfo;				/**< PA情報									*/
}CMT_CARDCNTBRINFO;

/**
 * @brief	TRA card control table
 * @note	Contents:	TRA card control status table \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
/* ========================================================================  */
/* カード制御状態テーブル													 */
/*  補足：制御コードについて(最新の制御状態を以下の形で保持)				 */
/*		┌───┬───┬───┬───┬───┬───┬───┬───┐	 */
/*		│L閉塞 │L     │L     │L     │R     │R     │R   ER│R   強│	 */
/*		│解除  │閉塞  │RST   │ALM   │ALM   │ERR   │RR回復│制閉塞│	 */
/*		├───┼───┼───┼───┼───┼───┼───┼───┤	 */
/*		│L     │L   ER│R     │      │R   N/│R   閉│R     │R     │	 */
/*		│ERR   │R回復 │診断  │予備  │E切替 │塞解除│閉塞  │RST   │	 */
/*		└───┴───┴───┴───┴───┴───┴───┴───┘	 */
/* ========================================================================  */

typedef struct{
	USHORT				exc_code;					/**< executing card control code   */
	USHORT				exc_itm;					/**< action code - blk/alm/...   */
	USHORT				exc_cmpblk;					/**< executing force block flag   */
	USHORT				cnt_kind;					/**< card control kind   */
	USHORT				cnt_itm_s3g;				/**< card control item   */
	USHORT				cnt_code;					/**< card control code   */
	USHORT				cnt_cmpblk;					/**< force block flag   */
	USHORT				blk_code;					/**< reserved BLK control code   */
	USHORT				err_code;					/**< reserved ERR control code   */
	USHORT				cnt_extitem_s3g;			/**< 拡張制御項目  */
	CMT_CARDCNTBRINFO	brInfo[CMD_MAX_BRNUM];		/**< ブランチ制御情報 */
}CMT_CARDCNTTBL;

/**
 * @brief	task state management table
 * @note	task state management table
 * 
  */
typedef struct{
	USHORT				rs_flg;					/**< RE supervise task status   */
	USHORT				rc_flg;					/**< RE control task status   */
	USHORT				wd_flg;					/**< Watch Dog timer task status   */
	USHORT				di_flg;					/**< DIA task status   */
}CMT_TSKSTAMNGTBL;

/* ========================================================================  */
/* ファイル情報報告テーブル													 */
/* ========================================================================  */
typedef struct
{
	USHORT		en_flg;							/**< ENフラグ					 */
	USHORT		target_data;					/**< 対象データ				 */
	USHORT		division_num;					/**< 分割数					 */
	USHORT		ng_fct;							/**< NG要因					 */
} CMT_FILEINFREP_TBL;

/**
 * @brief	current bootface table
 * @note	Contents:	current bootface table \n
 * 
  */
typedef struct
{
	USHORT		mpu_bootmen;						/**< MPU bootface		 */
	USHORT		fpga_bootmen;					    /**< CPRI-FPGA bootface	 */
	USHORT		apd3_bootmen;						/**< APD-FPGA bootface	 */
	USHORT		vupu_bootmen;						/**< VUPU bootface	 */
} CMT_USEBTFACETBL;


/**
 * @brief	TRA card inventory management table
 * @note	Contents:	TRA card inventory management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				cmx_cardname[6];		/**< Card name   */
	USHORT				cmx_pcbno1;				/**< Product number (Year/Month/Day)   */
	USHORT				cmx_pcbno2;				/**< Product number serial (high word)   */
	USHORT				cmx_pcbno3;				/**< Product number serial (low word)   */
	USHORT				cmx_pcbbyer;			/**< Product test Year/Month/Day (lowword of year)   */
	USHORT				cmx_pcbbday;			/**< Product test Year/Month/Day (month and day)   */
	USHORT				cmx_hver1;				/**< Hardware version number(high word)   */
	USHORT				cmx_hver2;				/**< Hardware version number(low word)   */
	USHORT				cmx_hver3;				/**< Hardware version number(low word)   */
	USHORT				cmx_mno[6];				/**< Card map number   */
	USHORT				cmx_f0sver[3];			/**< FLASH 0 plane software version   */
	USHORT				cmx_f1sver[3];			/**< FLASH 1 plane software version   */
	USHORT				cmx_fprom[4];			/**< Factory usage Address    */
	USHORT				cmx_mac_01;				/**< MAC address (high word)   */
	USHORT				cmx_mac_02;				/**< MAC address (low word)   */
	USHORT				cmx_mac_03;				/**< MAC address (card)   */
	USHORT				cmx_ipup;				/**< IP address (high word)   */
	USHORT				cmx_ipdwn;				/**< IP address (low word)   */
	USHORT				cmx_netmskup;			/**< Net mask (high word)   */
	USHORT				cmx_netmskdwn;			/**< Net mask (low word)   */
	USHORT				cmx_dfltrtrup;			/**< Default router (high word)   */
	USHORT				cmx_dfltrtrdwn;			/**< Default router (low word)   */
	USHORT				cmx_command_port;		/**< Command transfer port   */
	USHORT				cmx_file_port;			/**< File transfer port   */
	USHORT				cmx_port;				/**< Port number   */
	USHORT				cmx_erase_firm_flg;		/**< Flash erase flag for DL use (FIRM)   */
	USHORT				cmx_erase_fpga_flg;		/**< Flash erase flag for DL use (FPGA)   */
	USHORT				cmx_erase_vupu_flg;		/**< Flash erase flag for DL use (VUPU)   */
	USHORT				cmx_erase_apd_flg;		/**< Flash erase flag for DL use (APD-FPGA)   */
	USHORT				cmx_0fpgaver[3];		/**< Flash 0 plane FPGA version   */
	USHORT				cmx_1fpgaver[3];		/**< Flash 1 plane FPGA version   */
	USHORT				cmx_fpga_bootmen;		/**< FPGA boot info   */
	USHORT				cmx_mpu_bootmen;		/**< MPU boot info   */
	USHORT				cmx_apd3_bootmen;		/**< APD3 boot info   */
	USHORT				cmx_vupu_bootmen;		/**< VUPU boot info   */
	USHORT				cmx_tp_fstartup_flg;	/**< FPGA field TP force startup flag   */
	USHORT				cmx_syspara_rcv_flg;	/**< system parameter receive flg */
	USHORT				cmx_tilt_con_inf;		/**< TILT connection information   */
	USHORT				cmx_tra_inf_typ;		/**< RE side TRAINF type */
	USHORT				cmx_remain[197];		/**< Remain				*/
}CMT_TRAINVTBL;



/**
 * @brief	BRE-TERM inventory information management table
 * @note	Contents:	BRE-TERM inventory information management  \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				cmx_cardname[6];		/**< Card name   */
	USHORT				cmx_pcbno1;				/**< Product number (Year/Month/Day)   */
	USHORT				cmx_pcbno2;				/**< Product number serial (high word)   */
	USHORT				cmx_pcbno3;				/**< Product number serial (low word)   */
	USHORT				cmx_pcbbyer;			/**< Product test Year/Month/Day (lowword of year)   */
	USHORT				cmx_pcbbday;			/**< Product test Year/Month/Day (month and day)   */
	USHORT				cmx_hver;				/**< Hardware version number   */
	USHORT				cmx_mno[6];				/**< Card map number   */
}CMT_BRETERMINVTBL;

/**
 * @brief	shelf inventory information management table
 * @note	Contents:	shelf inventory information management  \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				cmx_cardname[6];		/**< Card name   */
	USHORT				cmx_pcbno1;				/**< Product number (Year/Month/Day)   */
	USHORT				cmx_pcbno2;				/**< Product number serial (high word)   */
	USHORT				cmx_pcbno3;				/**< Product number serial (low word)   */
	USHORT				cmx_pcbbyer;			/**< Product test Year/Month/Day (lowword of year)   */
	USHORT				cmx_pcbbday;			/**< Product test Year/Month/Day (month and day)   */
	USHORT				cmx_hver;				/**< Hardware version number   */
	USHORT				cmx_mno[6];				/**< Card map number   */
}CMT_SHELFINVTBL;

/**
 * @brief	cascade information table
 * @note	Contents:	cascade information table  \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				cscd_kind;
	USHORT				timer_state;
	USHORT				filever_disagreement;
}CMT_CASCADEINFOTBL;

/**
 * @brief	cascade version table
 * @note	Contents:	cascade version table  \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	UINT				dl_flg;
}CMT_CASCADEVERTBL;

/**
 * @brief	System parameter management table
 * @note	Contents: System parameter management table \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
  */
typedef struct{
	USHORT				cpri_sttimer;						/**< CPRI state stability timer  */
	USHORT				hls_timer;							/**< health check request timer  */
	USHORT				hls_unconfcnt;						/**< health check request unconfirmation counter  */
	USHORT				sys_timer;							/**< system parameter sending confirmation timer  */
	USHORT				firm_timer;							/**< firmware sending confirmation timer  */
	USHORT				sec_chip0;							/**< sector chip revision (#0)  */
	USHORT				sec_chip1;							/**< sector chip revision (#1)  */
	USHORT				oudevt_rcvtimer;					/**< external equipment receiving confirmation timer  */
	USHORT				fldmt_cttimer;						/**< FLD-MT connection confirmation timer  */
	USHORT				fldmt_rltimer;						/**< FLD-MT release confirmation timer  */
	USHORT				fldmt_dtcd_timer;					/**< FLD-MT data command receiving confirmation timer  */
	USHORT				fldmt_dtfl_timer;					/**< FLD-MT data file receiving confirmation timer  */
	USHORT				svmt_cttimer;						/**< SV-MT connection confirmation timer  */
	USHORT				svmt_rltimer;						/**< SV-MT release confirmation timer  */
	USHORT				svmt_rcvtimer;						/**< SV-MT data receiving confirmation timer  */
	USHORT				tx_div_value_3G;					/**< tx diversity setting value for 3G */
	USHORT				pri_sys_info;						/**< RE primary system information  */
	USHORT				s3g_bandwidth;						/**< S3G bandwidth  */
	USHORT				car_wk_flg;							/**< 3G carrier#1-4 working flag 0x00001111  */
	USHORT				tx_sys_num;							/**< tx system number  */
	USHORT				sec_chip234;						/**< sector chip revision (#2,#3,#4)  */
	USHORT				sec_chip567;						/**< sector chip revision (#5,#6,#7)  */
	USHORT				reserveE[CMD_NUM2];					/**< reserve for system parameter E  */
	USHORT				car1_wk_frq;						/**< 3G carrier#1 working frequency  */
	USHORT				car2_wk_frq;						/**< 3G carrier#2 working frequency  */
	USHORT				car3_wk_frq;						/**< 3G carrier#3 working frequency  */
	USHORT				car4_wk_frq;						/**< 3G carrier#4 working frequency  */
	USHORT				mtExt_data_3g_flg;					/**< 3G MT signaling/External equipment data corresponse flag (TBD)  */
	USHORT				syspara_wait_timer;					/**< システムパラメータ転送待ちタイマ  */
	USHORT				firm_sndtimer;						/**< ファームウェア送信確認タイマ（更新用）  */
	USHORT				reserveF;							/**< reserve for system parameter F  */
}CMT_SYSMNGTBL;

/* ========================================================================  */
/* TRA監視状態管理テーブル													 */
/* ========================================================================  */
typedef struct
{
	USHORT		tra_surv;			/**< TRA 監視							 */
	USHORT		dummy;				/**< dummy領域							 */
} CMT_TRASURVTBL;

/**
 * @brief	Toffset management table
 * @note	Contents:	Toffset management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				toffset;			/**< toffset value   */
	USHORT				ul_delay;			/**< uplink delay value   */
	USHORT				dl_delay;			/**< downlink delay value   */
	USHORT				dummy;				/**< dummy   */
}CMT_TOFFSETTBL;

/* ========================================================================  */
/* RE共通部情報共通管理テーブル								 */
/* ========================================================================  */

/**
 * @brief	RE common link information management table
 * @note	Contents:	RE common link information management \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				retype;								/**< RE type   */
	USHORT				selfslotno;							/**< self card slot no   */
	USHORT				traclsinfo[CMD_MAX_SYSNUM];			/**< self card active/negative information   */
	USHORT				reclscause[CMD_MAX_SYSNUM];			/**< RE common link setting cause   */
	USHORT				reclsseq[CMD_MAX_SYSNUM];			/**< RE common link setting sequence   */
	USHORT				actneginfo[CMD_MAX_SYSNUM];			/**< self card active/negative information before active led set   */
}CMT_RECLSMNGTBL;

/**
 * @brief	CPRI state management table
 * @note	CPRI state management table
 * 
  */
typedef struct{
	USHORT				cprilnkstano[CMD_MAX_SYSNUM];		/**< CPRI link state number   */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT				cpridscstbflg;						/**< 安定化 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	USHORT				cprista;							/**< CPRI state    */
	USHORT				hdlcsta;							/**< HDLC idle/active flag   */
	USHORT				stafflg;							/**< stateF flag   */
	USHORT				L2abnminfo;							/**< L2 link abnormal information   */
	USHORT				hcsta[CMD_MAX_SYSNUM];				/**< L3 health check state   */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT				cmuxrstinfo;   						/* CPRI-MUX含むリセット情報(1:CPRI-MUX含むリセット,0:以外のリセット) */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
}CMT_CPRILNKSTATBL;

/**
 * @brief	CPRI-MT/RE-MT Client information table
 * @note	Contents:	CPRI-MT/RE-MT Client information \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	USHORT				conn;								/**< Connection state   */
	USHORT				dummy;								/**< dummy			*/
	INT					tcplinkhdl;							/**< TCP link handle   */
}CMT_TBLCLIENTINFO;


/***
 *  @brief  MT address information table(16bytes)
 *  @note  Contents :  MT address information table(16bytes)
 *  
 * 
 */

typedef struct
{
	UCHAR				ipadd_1;				/**<  IPアドレス＃1			*/
	UCHAR				ipadd_2;				/**<  IPアドレス＃2			*/
	UCHAR				ipadd_3;				/**<  IPアドレス＃3			*/
	UCHAR				ipadd_4;				/**<  IPアドレス＃4			*/
	UCHAR				sbnetmsk_1;				/**<  サブネットマスク＃1		*/
	UCHAR				sbnetmsk_2;				/**<  サブネットマスク＃2		*/
	UCHAR				sbnetmsk_3;				/**<  サブネットマスク＃3		*/
	UCHAR				sbnetmsk_4;				/**<  サブネットマスク＃4		*/
	USHORT				cmdtrsport;				/**<  コマンド転送用ポート番号	*/
	USHORT				filtrsport;				/**<  ファイル転送用ポート番号	*/
	USHORT				portno;					/**<  ポート番号	(SV-MT) 			*/
	USHORT				dummy;					/**<  dummy					*/
}CMT_MTADDR_INFO;


/***
 *  @brief  MT address infomation table
 *  @note  Contents :  MT address infomation table
 *  
 * 
 */

typedef struct
{
	CMT_MTADDR_INFO mtaddr_sav[CMD_MAX_SYSNUM];	/**< MT address information table*/
	UCHAR reset_flg;		/**<  再設定ローカル・フラッグ	*/
	UCHAR dummy[CMD_NUM3];		/**<  dummy			*/
}CMT_MTSETMNG;


/**
 * @brief	TRA file information table
 * @note	Contents:	TRA file information \n
 * 			Initial	:	\n
 * 			Read	:	\n
 * 			Write	:	\n
 * 
  */
typedef struct{
	UINT				en_flg;					/**< EN flag   */
	UINT				file_type;				/**< file type   */
	UINT				div_no;					/**< division number   */
	UINT				dl_target;				/**< download target   */
}CMT_TRAFILEINFTBL;

/*! 
 * @brief S3G BTI setting type info
 */
/* the BTI setting type of S3G for up link */
typedef struct {
    USHORT                  s3g_20m;
	USHORT                  s3g_15m;
    USHORT                  s3g_10m;
    USHORT                  s3g_5m;
	USHORT					s3g_0m;
} CMT_S3GBTIUL_TBL;

/*! 
 * @brief S3G BTI setting type info
 */
/* the BTI setting type of S3G for down link */
typedef struct {
    USHORT                  s3g_20m;
	USHORT                  s3g_15m;
    USHORT                  s3g_10m;
    USHORT                  s3g_5m;
	USHORT					s3g_0m;
} CMT_S3GBTIDL_TBL;

/*! 
 * @brief 3G BTI setting type info
 */
/*-----the BTI setting type of 3G -----*/
typedef struct {
    USHORT                  cr1_3g;
	USHORT                  cr2_3g;
    USHORT                  cr3_3g;
    USHORT                  cr4_3g;
} CMT_3GBTI_TBL;

/*! 
 * @brief both 3G and S3G BTI setting type info
 */
/* the S3G BTI setting type of CPRI shall for up link */
typedef struct {
	USHORT					ba20;
	USHORT                  ba15;
	USHORT                  ba10;
    USHORT                  ba5;
	USHORT					ba0;
} CMT_CPRIS3GBTIUL_TBL;

/*! 
 * @brief both 3G and S3G BTI setting type info
 */
/* the 3G BTI setting type of CPRI shall for up link */
typedef struct {
	USHORT					cr14;
	USHORT                  cr13;
	USHORT                  cr12;
    USHORT                  cr11;
    USHORT                  cr0;
} CMT_CPRI3GBTIUL_TBL;
	
/*! 
 * @brief CPRI shall BTI setting type info
 */
/* the BTI S3G setting type of both 3G and S3G for down link */
typedef struct {
	USHORT					ba20;
	USHORT                  ba15;
	USHORT                  ba10;
    USHORT                  ba5;
	USHORT					ba0;
} CMT_CPRIS3GBTIDL_TBL;

/*! 
 * @brief CPRI shall BTI setting type info
 */
/* the BTI 3G setting type of both 3G and S3G for down link */
typedef struct {
	USHORT					cr14;
	USHORT                  cr13;
	USHORT                  cr12;
    USHORT                  cr11;
    USHORT                  cr0;
} CMT_CPRI3GBTIDL_TBL;

/*! 
 * @brief RF shall BTI setting type info
 */
/* the BTI 3G setting type of both 3G and S3G for down link */
typedef struct {
	USHORT					cr14;
	USHORT                  cr13;
	USHORT                  cr12;
    USHORT                  cr11;
    USHORT                  cr0;
} CMT_RF3GBTIDL_TBL;

/*! 
 * @brief TRA extend SV info
 */ 
typedef struct {
	USHORT					pasw_state;
}CMT_TRAEXTENDSV_TBL;

/*! 
 * @brief RSSI setting sequence for 3G S3G both connection
 */ 
typedef struct {
	USHORT					rx0_flg;			/**< for carrier 0 */
	USHORT					rx1_flg;		/**< for carrier 1 */
}CMT_RSSISEQ_TBL;


/**
 * @brief	system parameter which is set to hardware management table
 * @note	This table is for saving the system parameter which is set to hardware
 * 
 */
typedef struct{
	USHORT				cpri_sttimer;						/**< CPRI state stability timer  */
	USHORT				sec_chip0;							/**< sector chip revision (#0)  */
	USHORT				sec_chip1;							/**< sector chip revision (#1)  */
	USHORT				s3g_bandwidth;						/**< LTE bandwidth  */
	USHORT				car_wk_flg;							/**< 3G carrier#1-4 working flag   */
	USHORT				car1_wk_frq;						/**< 3G carrier#1 working frequency    */
	USHORT				car2_wk_frq;						/**< 3G carrier#2 working frequency  */
	USHORT				car3_wk_frq;						/**< 3G carrier#3 working frequency  */
	USHORT				car4_wk_frq;						/**< 3G carrier#4 working frequency  */
	USHORT				dummy;								/**< dummy		  					*/
}CMT_HWSETSYSTBL;


/************************/
/* 個別ログ管理型		*/
/************************/
/**
 * @brief	LOG infomation head	structure
 * @note	Contents:	LOG infomation head structure	\n
 * 
 */

typedef struct
{
	UINT		tskid ;				/**< 	起動元タスクID		*/
	UINT		event_no ;			/**< 	起動要因番号		*/
	USHORT		year ;				/**< 	書込発生年			*/
	UCHAR		month ;				/**< 	書込発生月			*/
	UCHAR		day ;				/**< 	書込発生月			*/
	UCHAR		hour ;				/**< 	書込発生時			*/
	UCHAR		min ;				/**< 	書込発生分			*/
	UCHAR		sec ;				/**< 	書込発生秒			*/
	UCHAR		msec ;				/**< 	書込発生ms			*/
	
}CMT_LOGHEAD_L3;

typedef struct
{
	USHORT				sig_kind;					/**<  信号種別				*/
	USHORT				conn_kind;					/**<  接続構成				*/
	USHORT				sec_chip[CMD_MAX_BRNUM];	/**<  Sector chip revision	*/
	USHORT				bandwidth_car1;				/**<  car1 BandWidth				*/
	USHORT				bandwidth_car2;				/**<  car2 BandWidth				*/
	USHORT				tx_sys_num;				    /**<  sys send number		*/
	USHORT				cpri_rate;				    /**<  cpri rate	*/
} CMT_SYSPARA_SET_INF;

typedef struct{
	UCHAR	stringVer[16];
	USHORT	usever;
	USHORT	stupver;
	USHORT	year;
	USHORT	mon_day;
}CMT_VERSION_DATA;

typedef struct
{
	USHORT ledCond;
	USHORT actLed;
	USHORT almLed;
	USHORT bsyLed;
	USHORT blkLed;
	USHORT useLed;
}CMT_LEDSETTBL;

//add Reform log by 20150608
typedef struct
{
	USHORT					year;
	UCHAR					month;
	UCHAR					day;
	UCHAR					hour;
	UCHAR					min;
	UCHAR					sec;
	UCHAR					msec;
	UINT					disp_no;
}CMT_CPRI_EXTENDED_FIELD;
//end Reform log by 20150608

/**
 * @brief   RE disconnection request
 * @note    起動要因:0x4007
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/* ========================================================================  */
/* MESSAGE :RE Disconnecion Request										*/
/* 起動要因:0x4007														 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				link_num;		/**< CPRI番号					 */
} CMT_TSKIF_CPRIDISCONN_REQ;

/**
 * @brief   RE disconnection response
 * @note    起動要因:0x4008
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/* ========================================================================  */
/* MESSAGE :RE Disconnection Response										*/
/* 起動要因:0x4008														 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				link_num;		/**< CPRI番号					 */
} CMT_TSKIF_CPRIDISCONN_RES;

/**
 * @brief   RE disconnection response
 * @note    起動要因:0x4009
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/* ========================================================================  */
/* MESSAGE :RE自律リセット抑止解除要求										*/
/* 起動要因:0x4009														 */
/* ========================================================================  */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ			 */
	USHORT				link_num;		/**< CPRI番号					 */
 	USHORT				reoff_flag;		/**< CPRI番号					 */
} CMT_TSKIF_RE_SLFRST_PRVTOFF_REQ;

 	
/* 起動要因（0ｘ4005,4006）L3 rct- RE rec間 */
/**
 * @brief   RE startup request
 * @note    起動要因:0x4005
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/* ======================================================================== */
/* MESSAGE :RE Startup Request												*/
/* 起動要因:0x4005															*/
/* ======================================================================== */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ		 */
	USHORT				link_num;				/**< CPRI番号				 */
	USHORT				system_type;			/**< lte or 3g					*/
} CMT_TSKIF_RESTRTUP_REQ;

/**
 * @brief   RE startup response
 * @note    起動要因:0x4006
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/* ======================================================================== */
/* MESSAGE :RE Startup Response												*/
/* 起動要因:0x4006															*/
/* ======================================================================== */
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< タスク共通ヘッダ		 */
	USHORT				link_num;				/**< CPRI番号				 */
	USHORT				system_type;			/**< lte or 3g					*/
	USHORT				result;					/**< lte or 3g					*/
} CMT_TSKIF_RESTRTUP_RES;

/**
 * @brief   REファイル更新要求
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :REファイル更新要求(CMT_TSKIF_REFILNEWREQ)						*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				sys_type;				/**< Systerm type           */
	USHORT				kind;					/**< download kind (0:RE startup/1:API) */
}CMT_TSKIF_REFILNEWREQ;

/**
 * @brief   REファイル更新応答
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :REファイル更新応答(CMT_TSKIF_REFILNEWRES)						*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				result_code;			/**< 結果コード				*/
	USHORT				reset_jdg;				/**< REリセット要否			*/
	USHORT				dummy;					/**< ダミー領域				*/
}CMT_TSKIF_REFILNEWRES;

/**
 * @brief   RE更新停止指示
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :RE更新停止指示(CMT_TSKIF_RENEWSTPNTC)				*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				systerm;				/**< system                 */
}CMT_TSKIF_RENEWSTPNTC;

/**
 * @brief   REファイル更新中止要求
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :REファイル更新中止要求(CMT_TSKIF_REFILNEWSTP)					*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				systerm;				/**< system                 */
}CMT_TSKIF_REFILNEWSTP;

/**
 * @brief   REシステムパラメータ更新要求
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :REシステムパラメータ更新要求(CMT_TSKIF_RESYSNEWREQ)				*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				dummy;					/**< ダミー領域				*/
}CMT_TSKIF_RESYSNEWREQ;

/**
 * @brief   REシステムパラメータ更新応答
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :REシステムパラメータ更新応答(CMT_TSKIF_RESYSNEWRES)				*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				result_code;			/**< 結果コード				*/
	USHORT				systype;				/**< 3g/lte                 */
	USHORT				dummy;					/**< ダミー領域				*/
}CMT_TSKIF_RESYSNEWRES;

/**
 * @brief   CPRI信号受信通知(信号種別取得用)
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ		*/
	USHORT					cpri_no;			/**< CPRI信号受信情報部		*/
}CMT_TSKIF_CPRINOGET;

/**
 * @brief   運用中REファイル更新開始通知
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :運用中REファイル更新開始通知(CMT_TSKIF_UREFILNEWSTAREQ)			*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
}CMT_TSKIF_UREFILNEWSTAREQ;

/**
 * @brief   運用中REファイル更新開始応答
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :運用中REファイル更新開始応答(CMT_TSKIF_UREFILNEWSTARES)			*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				result_code;			/**< 結果コード				*/
}CMT_TSKIF_UREFILNEWSTARES;

/**
 * @brief   運用中REファイル更新完了通知
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
/****************************************************************************/
/* MESSAGE :運用中REファイル更新完了通知(CMT_TSKIF_UREFILNEWFINRES)			*/
/****************************************************************************/
typedef struct
{
	CMT_TSKIF_HEAD		head;					/**< Message common header	*/
	USHORT				cpri_no;				/**< CPRI番号				*/
	USHORT				result_code;			/**< 結果コード				*/
}CMT_TSKIF_UREFILNEWFINRES;

/* @}  */
#endif	/*M_CM_TYP */
