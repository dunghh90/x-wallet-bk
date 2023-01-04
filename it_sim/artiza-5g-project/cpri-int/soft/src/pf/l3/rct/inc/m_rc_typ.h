/********************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		m_rc_typ.h
 *  @brief	Maker MT manage task Common structure
 *  @date   2008/07/29 FFCS)linlj create
 *  @date   2011/05/13 FJT)koshida modify for CeNB-F-073-008(F)(REにおける故障発生時の機能部状態報告)
 *  @date   2012/03/15 ALP)Yamamoto 12A-01-006/M-S3G-eNBSYS-02601：SLC-RREにおけるALM/ERRコード処理変更について
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
  */
/********************************************************************************/



/* 二重取込み防止 */
#ifndef		M_RC_TYP
#define		M_RC_TYP

/* ======================================================================== */
/* RE制御タスク内テーブル													*/
/* ======================================================================== */

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
	UCHAR			setFlg;		/* ダミー1         	*/
	UCHAR			month;		/* 月              	*/
	UCHAR			day;		/* 日              	*/
	UCHAR			hour;		/* 時              	*/
	UCHAR			min;		/* 分              	*/
	UCHAR			sec;		/* 秒              	*/
	UCHAR			msec;		/* ミリ秒          	*/
	UCHAR			sndrcvFlg;	/* 送信/受信フラグ 	*/
	UINT			msgId;		/* イベントID      	*/
	USHORT			taskId;		/* タスクID        	*/
	USHORT			sig_kind;	/* 信号種別        	*/
}RCT_LOGHEAD_T ;

/**
 * @brief	LOG infomation	structure
 * @note	Contents:	LOG infomation structure	\n
 * 
 */

typedef struct
{
	RCT_LOGHEAD_T		 loghead ;				/**< 	ログヘッダ部			*/
	UCHAR				 sv_tbl1[RCD_LOG_SV1_SIZE] ;/**< 	セーブ1面			*/
} RCT_LOG_T;

/****************************************************/
/* REカード状態作成Work領域テーブル型				*/
/****************************************************/
/**
 * @brief	version name convert	structure
 * @note	Contents:	version name convert structure	\n
 * 
 */
typedef struct
{
	USHORT	validFlg;							/**<  有効フラグ				*/
	USHORT	fbAlmSta;							/**<  機能部故障状態			*/
	USHORT	fbBaseSVItem;						/**<  機能部基本監視項目		*/
	USHORT	fbExtrSVItem;						/**<  機能部拡張監視項目		*/
//TD-SRE
	USHORT	brBaseSVItem[CMD_NUM4];				/**<  機能部BR基本監視項目	*/
	USHORT	brExtrSVItem[CMD_NUM4];				/**<  機能部BR拡張監視項目	*/
//TD-SRE
	USHORT  brAlmFlg[CMD_NUM4];					/*Use check branch if alm or not*/
} RCT_RECARDSTAWORK_T ;

/***************************************/
/*TDD-SRE RCT_CARDFB_DEVKIND_T*/
/****************************************/
/*!
 * @brief   card FB define table
 */
typedef struct{
    USHORT cardFbNum;                                       /**< fb number                                                    */
    USHORT cardFbKind[CMD_NUM7];                            /**< fb kind                                                      */
    USHORT cardFbBit;                                       /**< card FB bit(0x0Fb0)                                          */
}RCT_CARDFB_DEVKIND_T;

/*!
 * @brief   REキャリアALM の詳細情報
 */
typedef struct{
    USHORT almcd;                                           /**< alarm code                                                   */
    USHORT rcd_fb;                                          /**< FB ID                                                        */
    USHORT br_bits;                                         /**< Alarm Br Bits                                                */
    USHORT rcd_almcd;                                       /**< RCD alarm code                                               */
}RCT_CARDFB_BR_CARALM_T;

/*!
 * @brief   TRX設定パラメータテーブル(LTE)
 */
typedef struct
{
    CMT_TSKIF_HEAD          head;                           /**< タスク共通ヘッダ                                             */
    CMT_CPRISIGRCV_INF      cprircv_inf;                    /**< CPRI信号受信情報部                                           */
    T_TRXSET_PARA_LTE       cpridat_trxsetreq;              /**< CPRI信号データ部                                             */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXSET_LTE_REQ ;

/*!
 * @brief   TRX設定パラメータテーブル(3G)
 */
typedef struct
{
    CMT_TSKIF_HEAD          head;                           /**< タスク共通ヘッダ                                             */
    CMT_CPRISIGRCV_INF      cprircv_inf;                    /**< CPRI信号受信情報部                                           */
    T_TRXSET_PARA_3G        cpridat_trxsetreq;              /**< CPRI信号データ部                                             */
}__attribute__ ((packed)) CMT_TSKIF_CPRIRCV_TRXSET_3G_REQ ;

/**
 * @brief    データコマンド情報
 * 
 */
typedef struct
{
	/**< 全送信データ数			 */
	USHORT				sndalldat_num;
	/**< 送信データ順序番号		 */
	USHORT				snddatseq_no;
}__attribute__ ((packed)) RCT_DATA_CMD_T;

/**
 * @brief    データコマンド受信情報
 * 
 */
typedef struct
{
	/**< CPRI番号					 */
	USHORT				link_num;
	/**< 全受信データ数			 */
	USHORT				rcvalldat_num;
	/**< 受信データ順序番号		 */
	USHORT				rcvdatseq_no;
}__attribute__ ((packed)) RCT_RCV_DATA_CMD_T;

/**
 * @brief  外部装置データ
 * @date   2015/10/02 TDI)satou 「受信元CPRIリンク番号」を追加
 */
typedef struct
{
	/** 信号種別 */
	USHORT		wSignalType;
 	/** 送信バイト数 */
	USHORT		wSize;
	/** 受信元CPRIリンク番号 */
	USHORT      wCpriNum;
	/** 送信データ */
    UCHAR		cData[2048];
}__attribute__ ((packed)) RCT_EXT_DATA_T;

/**
 * @brief    外部装置データバッファ
 * 
 * 外部装置データ送信要求処理の競合に後発の外部装置データ送信要求信号をバッファリングする。
 * 先発の外部装置データ送信要求処理が完了時に、バッファリングされている外部装置データ送信要求処理を行う
 * 
 */
typedef struct
{
	/** バッファリング数 */
	USHORT		wBufferNum;
	/** 書き込みIDX */
	USHORT		wWriteId;
	/** 読み込みIDX */
	USHORT		wReadId;
	/** 外部装置データ送信 */
	RCT_EXT_DATA_T		tExtDataSnd[30];
}__attribute__ ((packed)) RCT_EXT_DATA_SND_T;

/**
 * @brief    外部装置データバッファ
 * 
 * 外部装置データ受信要求処理の競合に後発の外部装置データ受信要求信号をバッファリングする。
 * 先発の外部装置データ受信要求処理が完了時に、バッファリングされている外部装置データ受信要求処理を行う。
 * 
 */
typedef struct
{
	/** バッファリング数 */
	USHORT		wBufferNum;
	/** 書き込みIDX */
	USHORT		wWriteId;
	/** 読み込みIDX */
	USHORT		wReadId;
	/** 外部装置データ受信 */
	RCT_EXT_DATA_T		tExtDataRcv[30];
}__attribute__ ((packed)) RCT_EXT_DATA_RCV_T;

/**
 * @brief	REカード情報報告管理テーブル
 * 
 * @note	RECから(3G)REカード情報報告要求を受信時に、配下REのREカード情報報告を
 * @note	収集、合算してRECにREカード情報報告応答を送信するための管理テーブル
 * 
 */
typedef struct
{
	/** 処理中状態  0:未処理(初期値) 1:処理中 */
	USHORT		wRun;

// [16B] add start
    // REカード情報報告要求の送信数
    USHORT      req_num;
// [16B] add end

	/** 分配収集対象REリスト  CPRIリンク数分の配列 */
	/**                       0:対象外(初期値) 1:対象 */
// [16B] chg start
//	USHORT		wReList[D_RRH_CPRINO_RE_MAX];
	USHORT		wReList[D_RRH_CPRINO_RE_MAX][CMD_MAX_CARDNUM];
// [16B] chg end

	/** 有効データ数  0:配下REから収集した有効データなし */
	/**               1:配下REから収集した有効データあり */
	USHORT		wDataValid;
	
	
	/** リソース数不一致  0:上下のリソース数の不一致無し */
	/**                   1:上下のリソース数の不一致有り */
	USHORT		wRscMismatch;
	
	/** 配下REから収集したカード情報報告を合算した情報*/
	CMT_CPRIF_CARDINFRES		tCarInfo;
}__attribute__ ((packed)) RCT_CARD_INFO_DATA_T;


#endif	/* M_RC_TYP */
