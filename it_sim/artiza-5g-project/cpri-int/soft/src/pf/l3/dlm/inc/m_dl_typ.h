/*!
 * @skip  $ld:$
 * @file  M_dl_typ.h
 * @brief Structure definition for DLM thread.
 * @date  2008/07/30 FFCS)Wuh Create
 * @date  2009/07/02 FFCS)Tangj Modify for CR-00055-000
 *                   3G VUPU DL case, modify the DLT_MAKER_RESERVE_3G
 * @date  2010/05/06 FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 * @date  2011/02/25 FJT)Tokunaga  modify for ECO-800M-RRE / 1.5G-RRE対応
 * @date  2011/09/16 COMAS)Uchida  modify for 11B-02-005：VA提案の2G/800MのSLCアーキ適用対応
 * @date  2015/10/27 TDIPS)sasaki modify for FHM DLM管理テーブルのバージョン情報のコメント修正
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2015
 */

/* 二重取込み防止 */
#ifndef		M_DL_TYP
#define		M_DL_TYP

/**
 * @brief   table structure define for global event code and task internal event code conversion
 */
typedef struct
{
    UINT        gbleventno;			/**< global event code */
	USHORT		addinfo1;			/**< CPRI信号 or TimerId */ 
	USHORT		addinfo2;			/**< TimerId Max */ 
}DLT_EVENT_CTBL;

/****************************************************************************************************************************/
/* ダウンロード管理管理型																									*/
/****************************************************************************************************************************/
/*! 
 * @brief download mangement table 
 */
typedef struct {
	UINT		total_size;																		/* トータルデータサイズ		*/
	UINT		rcvdata_size;																	/* 総受信データサイズ		*/
	USHORT		div_no;																			/* 分割番号					*/
	UCHAR		file_name[DLD_FILENAME_NUM];													/* ファイル名				*/
	USHORT		dummy;																			/* dummy領域				*/
} DLT_DLMNG_TBL;


/****************************************************************************************************************************/
/* データ展開テーブル																										*/
/****************************************************************************************************************************/
/*! 
 * @brief CPRIFPGA info
 */

/*! 
 * @brief deploy data info
 */
typedef struct {
    UINT                  mpu_size;                /* the data size of MPU size */
	UINT                  cprifpgarre_size;        /* the data size of CPRIFPGA for RRE */
	UINT                  cprifpga_size;           /* the data size of CPRIFPGA for BRE/LRE */
	UINT                  apdfpgarre_size;         /* the data size of APD for RRE */
	UINT                  apdfpga_size;            /* the data size of APD for BRE/LRE */
} DLT_DEVDATA_HEAD;

/*! 
 * @brief S3G BTI setting type info
 */
/*-----the BTI setting type of S3G -----*/
typedef struct {
    USHORT                  s3g_20m;
	USHORT                  s3g_15m;
    USHORT                  s3g_10m;
    USHORT                  s3g_5m;
} DLT_S3GBTI_TBL;

/*! 
 * @brief 3G BTI setting type info
 */
/*-----the BTI setting type of 3G -----*/
typedef struct {
    USHORT                  cr1_3g;
	USHORT                  cr12_3g;
    USHORT                  cr13_3g;
    USHORT                  cr14_3g;
} DLT_3GBTI_TBL;

/*! 
 * @brief both 3G and S3G BTI setting type info
 */
/*-----the BTI setting type of both 3G and S3G-----*/
typedef struct {
    USHORT                  cr1_15m;
	USHORT                  cr1_10m;
    USHORT                  cr12_10m;
    USHORT                  cr1_5m;
	USHORT                  cr12_5m;
	USHORT                  cr13_5m;
} DLT_3GS3GBTI_TBL;


/**
 * @brief	data reserve common structure for DLM receive data head
 * @note	Contents: data reserve common structure \n
 * 
 */

/*************************************************************************** */
/* データ格納テーブル(ヘッダ)型	DLM共通										 */
/*************************************************************************** */

typedef struct
{
	USHORT		 file_sum;                  /**<   結合数                */
	USHORT		 mpu_mj_ver;                /**<   MPUファームMJバージョン    */
	USHORT		 mpu_mn_ver;                /**<   MPUファームMNバージョン    */
	USHORT		 cpri_mj_ver;               /**<   CPRI-FPGA(RRE用)MJバージョン   */
	USHORT		 cpri_mn_ver;               /**<   CPRI-FPGA(RRE用)MNバージョン   */
	UINT         mpu_size;					/**<   MPUファームsize       */
	UINT         cpri_size;					/**<   CPRI-FPGA(RRE用)size       */
	UCHAR		 reserve[DLD_RCVDAT_MKRSV_DEBUG_3G];	/**< 空き					 */
	
}DLT_MAKER_RESERVE_DEBUG_3G  ;

/**
 * @brief   receive data head for debug 3G
 */
typedef struct {
	UINT		pgm_size;						/**< プログラムサイズ			 */
	USHORT		chk_sum;						/**< チェックサム				 */
	USHORT		mk_jdg;							/**< メーカ識別子				 */
	UCHAR		mk_name[CMD_NUM16];				/**< メーカ名称				 */
	UCHAR		file_kind[CMD_NUM16];			/**< ファイル種別名称			 */
	USHORT		file_type;						/**< ファイルタイプ			 */
	USHORT		mjr_ver;						/**< メジャーバージョン		 */
	USHORT		min_ver;						/**< マイナーバージョン		 */
	USHORT		revision;						/**< リビジョン				 */
	USHORT		year;							/**< 年						 */
	UCHAR		month;							/**< month    */
	UCHAR		day;							/**< 月日						 */
	USHORT		mk_jdg2;                        /**< 第二メーカ識別子*/
	UCHAR		reserve[DLD_RCVDAT_RSV];		/**< 予約領域					 */
	DLT_MAKER_RESERVE_DEBUG_3G mk_reserve;				/**< メーカ固有領域			 */
	UCHAR		data[DLD_DATA_NUM_3G];				/**< 圧縮データ				 */
} DLT_RCVDATA_HEAD_DEBUG_3G;

/**
 * @brief   maker reserve area of RE file head structure for 3G (filenum =3 case)
 * @note	Contents: Maker reserve area structure for 3G (filenum =3 case)
 * 
 */

typedef struct
{
	USHORT		 file_sum;                  /**<   結合数                */
	USHORT		 mpu_mj_ver;                /**<   MPUファームMJバージョン    */
	USHORT		 mpu_mn_ver;                /**<   MPUファームMNバージョン    */
	USHORT		 cprirre_mj_ver;            /**<   CPRI-FPGA(RRE用)MJバージョン    */
	USHORT		 cprirre_mn_ver;            /**<   CPRI-FPGA(RRE用)MNバージョン    */
	USHORT		 cpri_mj_ver;               /**<   CPRI-FPGA(BRE/LRE用)MJバージョン   */
	USHORT		 cpri_mn_ver;               /**<   CPRI-FPGA(BRE/LRE用)MNバージョン   */
	UINT         mpu_size;					/**<   MPUファームsize       */
	UINT         cprirre_size;				/**<   CPRI-FPGA(RRE用)size       */
	UINT         cpri_size;					/**<   CPRI-FPGA(BRE/LRE用)size       */
	UCHAR		 reserve[DLD_RCVDAT_MKRSV_3G_FILNUM3];	/**< 空き					 */
	
}DLT_MAKER_RESERVE_3G_FILNUM3  ;

/**
 * @brief   maker reserve area of RE file head structure for 3G (filenum =4 case)
 * @note	Contents: Maker reserve area structure for 3G (filenum =4 case)
 * 
 */

typedef struct
{
	USHORT		 file_sum;                  /**<   結合数                */
	USHORT		 mpu_mj_ver;                /**<   MPUファームMJバージョン    */
	USHORT		 mpu_mn_ver;                /**<   MPUファームMNバージョン    */
	USHORT		 cprirre_mj_ver;            /**<   CPRI-FPGA(RRE用)MJバージョン    */
	USHORT		 cprirre_mn_ver;            /**<   CPRI-FPGA(RRE用)MNバージョン    */
	USHORT		 cpri_mj_ver;               /**<   CPRI-FPGA(BRE/LRE用)MJバージョン   */
	USHORT		 cpri_mn_ver;               /**<   CPRI-FPGA(BRE/LRE用)MNバージョン   */
	USHORT		 vupu_mj_ver;				/**<   VUPU MJバージョン    */
	USHORT		 vupu_mn_ver;				/**<   VUPU MNバージョン    */
	UINT         mpu_size;					/**<   MPUファームsize       */
	UINT         cprirre_size;				/**<   CPRI-FPGA(RRE用)size       */
	UINT         cpri_size;					/**<   CPRI-FPGA(BRE/LRE用)size       */
	UINT         vupu_size;					/**<   VUPU size        */
	USHORT		 mpu_cstdwn;				/**<   MPUコストダウン有無		*/
	USHORT		 hrd_cstdwn;				/**<   Hardコストダウン有無		*/
	USHORT		 file_sum_lsi;				/**<   LSI版用ファイル数       */
	USHORT		 cprirre_lsi_mj_ver;		/**<   CPRI-FPGA(RRE LSI用)MJバージョン    */
	USHORT		 cprirre_lsi_mn_ver;		/**<   CPRI-FPGA(RRE LSI用)MNバージョン    */
	USHORT		 vupu_lsi_mj_ver;			/**<   VUPU(RRE LSI用)MMJバージョン    */
	USHORT		 vupu_lsi_mn_ver;			/**<   VUPU(RRE LSI用)MMNバージョン    */
	UINT         cprirre_lsi_size;			/**<   CPRI-FPGA(RRE LSI用)size       */
	UINT		 vupu_lsi_size;				/**<   VUPU(RRE LSI用)size       */
	USHORT		 mpu_eco;					/**<   MPU  eCO-800M,1.5G 有無	*/
	USHORT		 hrd_eco;					/**<   Hard eCO-800M,1.5G 有無	*/
	USHORT		 dummy;						/**<   ダミー					*/
	USHORT		 mpu_slc;					/**<   MPU  SLC-RRE 有無		*/
	USHORT		 hrd_slc;					/**<   Hard SLC-RRE 有無		*/
	UINT		 dummy02;					/**<   ダミー					*/
	USHORT		 mpu_tra_cpri;				/**<   MPU  TRA CPRI CD 有無		*/
	USHORT		 hrd_tra_cpri;				/**<   Hard TRA CPRI CD 有無		*/
	UINT		 mpu2_1chp_flg;
	UINT		 mpu2_1chp_off;
	UINT		 mpu2_1chp_size;
	UCHAR		 reserve[DLD_RCVDAT_MKRSV_3G_FILNUM4 - CMD_NUM12];	/**< 空き					 */
}DLT_MAKER_RESERVE_3G_FILNUM4  ;

/**
 * @brief   maker reserve area of RE file head union structure for 3G 
 * @note	Contents: Maker reserve area union structure for 3G 
 * 
 */

typedef union
{		
	USHORT		 file_sum;                  			/**<   結合数                */
	DLT_MAKER_RESERVE_3G_FILNUM3	maker_3g_filnum3;	/**<  maker reserve for 3G with file_num=3 */
	DLT_MAKER_RESERVE_3G_FILNUM4	maker_3g_filnum4;	/**<  maker reserve for 3G with file_num=4 */

}DLT_MAKER_RESERVE_3G;

/**
 * @brief   maker reserve area of RE file head structure for S3G
 * @note	Contents: Maker reserve area structure for S3G 
 * 
 */

typedef struct
{
	USHORT		 file_sum;                  /**<   結合数                */
	USHORT		 mpu_mj_ver;                /**<   MPUファームMJバージョン    */
	USHORT		 mpu_mn_ver;                /**<   MPUファームMNバージョン    */
	USHORT		 cprirre_mj_ver;            /**<   CPRI-FPGA(RRE用)MJバージョン    */
	USHORT		 cprirre_mn_ver;            /**<   CPRI-FPGA(RRE用)MNバージョン    */
	USHORT		 cpri_mj_ver;               /**<   CPRI-FPGA(BRE/LRE用)MJバージョン   */
	USHORT		 cpri_mn_ver;               /**<   CPRI-FPGA(BRE/LRE用)MNバージョン   */
	USHORT		 apd_mj_ver;                /**<   APD-FPGA MJバージョン    */
	USHORT		 apd_mn_ver;                /**<   APD-FPGA MNバージョン    */
	USHORT		 vupu_mj_ver;				/**<   VUPU MJバージョン    */
	USHORT		 vupu_mn_ver;				/**<   VUPU MNバージョン    */
	UINT         mpu_size;					/**<   MPUファームsize       */
	UINT         cprirre_size;				/**<   CPRI-FPGA(RRE用)size       */
	UINT         cpri_size;					/**<   CPRI-FPGA(BRE/LRE用)size       */
	UINT         apd_size;					/**<   APD-FPGA size       */
	UINT		 vupu_size;					/**<   VUPU size       */
	USHORT		 mpu_cstdwn;				/**<   MPUコストダウン有無		*/
	USHORT		 hrd_cstdwn;				/**<   Hardコストダウン有無		*/
	USHORT		 file_sum_lsi;				/**<   LSI版用ファイル数       */
	USHORT		 cprirre_lsi_mj_ver;		/**<   CPRI-FPGA(RRE LSI用)MJバージョン    */
	USHORT		 cprirre_lsi_mn_ver;		/**<   CPRI-FPGA(RRE LSI用)MNバージョン    */
	USHORT		 vupu_lsi_mj_ver;			/**<   VUPU(RRE LSI用)MMJバージョン    */
	USHORT		 vupu_lsi_mn_ver;			/**<   VUPU(RRE LSI用)MMNバージョン    */
	UINT         cprirre_lsi_size;			/**<   CPRI-FPGA(RRE LSI用)size       */
	UINT		 vupu_lsi_size;				/**<   VUPU(RRE LSI用)size       */
	USHORT		 mpu_eco;					/**<   MPU  eCO-800M,1.5G 有無	*/
	USHORT		 hrd_eco;					/**<   Hard eCO-800M,1.5G 有無	*/
	USHORT		 dummy;						/**<   ダミー					*/
	USHORT		 mpu_slc;					/**<   MPU  SLC-RRE 有無		*/
	USHORT		 hrd_slc;					/**<   Hard SLC-RRE 有無		*/
	UINT         cpri_tra_cpri_size;		/**<   CPRI-FPGA(TRA CPRI CD用)size	*/
	USHORT		 mpu_tra_cpri;				/**<   MPU  TRA CPRI CD 有無		*/
	USHORT		 hrd_tra_cpri;				/**<   Hard TRA CPRI CD 有無		*/
	UINT		mpu2_1chp_flg;
	UINT		mpu2_1chp_off;
	UINT		mpu2_1chp_size;
	UINT		cpri2_1chp_flg;
	UINT		cpri2_1chp_off;
	UINT		cpri2_1chp_size;
	UCHAR		 reserve[DLD_RCVDAT_MKRSV_S3G - CMD_NUM24];	/**< 空き					 */
}DLT_MAKER_RESERVE_S3G  ;

/**
 * @brief	DLM receive data head structure
 * @note	Contents: DLM receive data head for 3G\n
 * 
 */

typedef struct {
	UINT		pgm_size;						/**< プログラムサイズ			 */
	USHORT		chk_sum;						/**< チェックサム				 */
	USHORT		mk_jdg;							/**< メーカ識別子				 */
	UCHAR		mk_name[CMD_NUM16];				/**< メーカ名称				 */
	UCHAR		file_kind[CMD_NUM16];			/**< ファイル種別名称			 */
	USHORT		file_type;						/**< ファイルタイプ			 */
	USHORT		mjr_ver;						/**< メジャーバージョン		 */
	USHORT		min_ver;						/**< マイナーバージョン		 */
	USHORT		revision;						/**< リビジョン				 */
	USHORT		year;							/**< 年						 */
	UCHAR		month;							/**< month    */
	UCHAR		day;							/**< 月日						 */
	USHORT		mk_jdg2;                        /**< 第二メーカ識別子*/
	UCHAR		reserve[DLD_RCVDAT_RSV];		/**< 予約領域					 */
	DLT_MAKER_RESERVE_3G mk_reserve;		/**< メーカ固有領域			 */
	UCHAR		data[DLD_DATA_NUM_3G];				/**< 圧縮データ				 */
} DLT_RCVDATA_HEAD_3G;

/**
 * @brief	DLM receive data head structure
 * @note	Contents: DLM receive data head for s3G\n
 * 
 */

typedef struct {
	UINT		pgm_size;						/**< プログラムサイズ			 */
	USHORT		chk_sum;						/**< チェックサム				 */
	USHORT		mk_jdg;							/**< メーカ識別子				 */
	UCHAR		mk_name[CMD_NUM16];				/**< メーカ名称				 */
	UCHAR		file_kind[CMD_NUM16];			/**< ファイル種別名称			 */
	USHORT		file_type;						/**< ファイルタイプ			 */
	USHORT		mjr_ver;						/**< メジャーバージョン		 */
	USHORT		min_ver;						/**< マイナーバージョン		 */
	USHORT		revision;						/**< リビジョン				 */
	USHORT		year;							/**< 年						 */
	UCHAR		month;							/**< month    */
	UCHAR		day;							/**< 月日						 */
	USHORT		mk_jdg2;                        /**< 第二メーカ識別子*/
	UCHAR		reserve[DLD_RCVDAT_RSV];		/**< 予約領域					 */
	DLT_MAKER_RESERVE_S3G mk_reserve;				/**< メーカ固有領域			 */
	UCHAR		data[DLD_DATA_NUM_S3G];				/**< 圧縮データ				 */
} DLT_RCVDATA_HEAD_S3G;

/** system parameterD,E,F check table */
typedef struct{
	USHORT min;								/**< 最小値					*/
	USHORT max;								/**< 最大値					*/
	USHORT typical;							/**< Typical設定値			*/
}DLT_SYSPRMCHK;

/**
 * @brief	running history
 * @note	Contents:running history	\n
 * 
 */
typedef struct
{
	UCHAR			setFlg;		
	UCHAR			month;		
	UCHAR			day;		
	UCHAR			hour;	
	UCHAR			min;	
	UCHAR			sec;	
	UCHAR			msec;	
	UCHAR			sndrcv_flg;	
	UINT			event_id;	
	USHORT			task_id;	
	USHORT			sig_kind;
}	DLT_RUNHIS_HEAD;



/**
 * @brief	running history information head
 * @note	Contents:running history information head	\n
 * 
 */
typedef struct
{
	DLT_RUNHIS_HEAD	head;
	CHAR 			body[40];	
}	DLT_RUNHIS_INF;

/**
 * @brief	running history information table
 * @note	Contents:running history information table	\n
 * 
 */
typedef struct
{
	UINT			write_idx;		
	DLT_RUNHIS_INF	inf[DLD_RUNHIS_FACENUM_MAX];
								
}DLT_RUNHIS_TBL;


/************************************************************************************************/
/* DLM管理テーブル																				*/
/************************************************************************************************/
/*! 
 * @brief DLM mangement table 
 */
typedef struct {
	USHORT		dlstart_factor;									/* 実行中DL開始要因				*/
	USHORT		link_inf;										/* 更新中リンク情報				*/
	USHORT		write_flg;										/* フラッシュ書込フラグ			*/
	USHORT		dlrefile_read_sta;								/* DL中REファイル読込状態		*/
	USHORT		dlrefile_inf;									/* DL中REファイル情報			*/
	USHORT		mk_jdg;											/* メーカ識別子					*/
	USHORT		mjr_ver;										/* メジャーバージョン(起動用)	*/
	USHORT		min_ver;										/* マイナーバージョン(起動用)	*/
	USHORT		usmjr_ver;										/* メジャーバージョン(運用中)	*/
	USHORT		usmin_ver;										/* マイナーバージョン(運用中)	*/
} DLT_DLM_TBL;

/************************************************************************************************/
/* 対REC情報テーブル																			*/
/************************************************************************************************/
/*! 
 * @brief REC information table 
 */
typedef struct {
	USHORT		rec_sta;									/* 対REC状態						*/
	USHORT		use_timer;									/* 使用中タイマ						*/
	ULONG		rcv_div_no;									/* 受信済み分割番号					*/
	ULONG		rcv_data_size;								/* 受信済みデータサイズ				*/
	USHORT		fileinf_rep_rslt;							/* ファイル情報報告結果				*/
	USHORT		filedata_trans_rslt;						/* ファイルデータ送信完了報告結果	*/
} DLT_REC_TBL;

/************************************************************************************************/
/* 対RE-FW情報テーブル																			*/
/************************************************************************************************/

/*! 
 * @brief RE-FW information table (REFW)
 */
typedef struct {
	USHORT		link_num;							/* CPRI#									*/
	USHORT		re_sta;								/* 対RE状態									*/
	USHORT		start_factor;						/* 更新要開始因								*/
	USHORT		mk_jdg;								/* メーカ識別子								*/
	USHORT		usmjr_ver;							/* メジャーバージョン(運用中)				*/
	USHORT		usmin_ver;							/* マイナーバージョン(運用中)				*/
	USHORT		mjr_ver;							/* メジャーバージョン(起動用)				*/
	USHORT		min_ver;							/* マイナーバージョン(起動用)				*/
	USHORT		use_timer;							/* 使用中タイマ								*/
	USHORT		fhmrefile_inf;						/* FHM保持REファイル情報					*/
	ULONG		trx_div_no;							/* 送信済み分割番号							*/
	ULONG		trx_data_size;						/* 送信済みデータサイズ						*/
	USHORT		fw_rep_rslt;						/* ファームウェアファイル報告結果			*/
	USHORT		fileinf_rep_rslt;					/* ファイル情報報告結果						*/
	USHORT		filedata_trans_rslt;				/* ファイルデータ送信完了報告結果			*/
	USHORT		fw_rep_tc;							/* 再送回数[ファームウェアファイル報告要求]	*/
	USHORT		fileinf_rep_tc;						/* 再送回数[ファイル情報報告通知]			*/
	USHORT		filedata_trans_tc;					/* 再送回数[ファイルデータ送信完了報告通知]	*/
} DLT_REFW_TBL;

/************************************************************************************************/
/* FHM保持REファイル情報																		*/
/************************************************************************************************/
/*! 
 * @brief FHM maintenance RE file information 
 */
typedef struct fhm_mng_re {
	USHORT		mk_jdg;											/* メーカ識別子					*/
	USHORT		mjr_ver;										/* メジャーバージョン			*/
	USHORT		min_ver;										/* マイナーバージョン			*/
	USHORT		re_conn_exist;									/* 同一メーカRE接続有無			*/
	ULONG		file_size;										/* ファイルサイズ				*/
	UCHAR		file_name[CMD_NUM20];							/* ファイル名					*/
    USHORT      year;                           				/* 年 */
    UCHAR       month;                          				/* 月 */
    UCHAR       day;                            				/* 日 */
	ULONG		div_num;										/* 分割数(=ファイルサイズ/10240)*/
	ULONG		file_addr;										/* ファイルデータ展開先アドレス	*/
} DLT_MTN_REINF;

#endif	/* M_DL_TYP */
