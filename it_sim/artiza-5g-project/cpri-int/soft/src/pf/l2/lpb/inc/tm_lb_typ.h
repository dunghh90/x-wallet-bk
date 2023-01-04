/*!
 * @skip    $Id$
 * @file    tm_lb_typ.h
 * @brief   LAPB 共通構造体ヘッダファイル
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/



#ifndef    TM_LB_TYP
#define    TM_LB_TYP

/* 走行履歴情報 */
/*!
 *  @brief   走行履歴情報
 *  @note    走行履歴情報
 */
typedef struct
{
	UCHAR			setFlg;                                  /*!< �?             */
	UCHAR			month;                                  /*!< �?             */
	UCHAR			day;                                    /*!< �?             */
	UCHAR			hour;                                   /*!< �?             */
	UCHAR			min;                                    /*!< �?             */
	UCHAR			sec;                                    /*!< �?             */
	UCHAR			msec;                                   /*!< ミリ�?         */
	UCHAR			sndrcv_flg;                             /*!< 送信/受信フラ�?*/
	UCHAR			link_sta;                               /*!< 信号種別        */
	UCHAR			task_sta;                               /*!< 信号種別        */
	USHORT			task_id;                                /*!< Source task ID        */
	USHORT			inter_event_id;                         /*!< internal event ID      */
	UINT			ext_event_id;                           /*!< イベントID      */
}	LBT_RUNHIS_INF;

/* 走行履歴情報テーブル */
/*!
 *  @brief   走行履歴情報テーブル
 *  @note    走行履歴情報テーブル
 */
typedef struct
{
	UINT			write_idx;                              /*!< 書込位置     */
	UINT			dummy[3];                               /*!< Dummy	    */
	LBT_RUNHIS_INF	inf[1024];            /*!< 情報*/
										/* 情報         */
}	LBT_RUNHIS_TBL;

											  /*******************************/
											  /*   FRMR LOG デー�? 格納テーブル  */
											  /*******************************/
/*!
 *  @brief   FRMR LOG デー�? 格納テーブル
 *  @note    FRMR LOG デー�? 格納テーブル
 */
struct	LBT_STSLOG_DAT
{
	T_RRH_SYSTEM_TIME	datetime;                                   /*!< datetime */
	UINT		sts_data[50];                    /*!< FRMR LOG デー�? 格納 */
};
											  /*******************************/
											  /*   FRMR LOG デー�? 格納テーブル  */
											  /*******************************/
/*!
 *  @brief   Abnormal LOG デー�? 格納テーブル
 *  @note    Abnormal LOG デー�? 格納テーブル
 */
struct	LBT_ABTLOG_DAT
{
	T_RRH_SYSTEM_TIME	datetime;                                   /*!< datetime */
	UINT		ab_data[10];                      /*!< L2 abort log area */
};
											  /*******************************/
											  /*   FRMR LOG デー�? 格納テーブル  */
											  /*******************************/
/*!
 *  @brief   FRMRデー�? 格納テーブル
 *  @note    FRMRデー�? 格納テーブル
 */
struct	LBT_FMRLOG_DAT
{
	T_RRH_SYSTEM_TIME	datetime;                                   /*!< datetime */
	UCHAR		fmr[3];                                     /*!< fmr */
	USHORT		link_num;                                   /*!< link_num */
};

#define D_ENDIAN_LITTLE
#ifdef D_ENDIAN_LITTLE

											  /*******************************/
											  /*   FRMRデー�? 格納テーブル  */
											  /*******************************/
/*!
 *  @brief   Control Field
 *  @note    Control Field
 */
union	FMR1_DAT
{
	/*!
	 *  @brief   Control Field
	 *  @note    Control Field
	 */
	struct	FMR1_INF                                        /*!< */
	{                                                       /*!< */
		unsigned	rej_ctl:8;                              /*!< */
		unsigned	zero:1;                                 /*!< */
		unsigned	vs:3;                                   /*!< */
		unsigned	n:1;                                    /*!< */
		unsigned	vr:3;                                   /*!< */
		unsigned	w:1;                                    /*!< */
		unsigned	x:1;                                    /*!< */
		unsigned	y:1;                                    /*!< */
		unsigned	z:1;                                    /*!< */
		unsigned	zero2:4;                                /*!< */
		unsigned	dummy:8;                                /*!< */
	}fmr1_inf;                                              /*!< */
	UCHAR	fmr1[4];                                        /*!< */
};

											  /*******************************/
											  /*        Control Field 1      */
											  /*******************************/
/*!
 *  @brief   Control Field
 *  @note    Control Field
 */
union CTL_FLD1
{
	UCHAR			ctl1_all;                               /*!< */
	/*!
	 *  @brief   Control Field
	 *  @note    Control Field
	 */
	struct CTL_IFRM8                                        /*!< */
	{                                                       /*!< */
		unsigned	i8zero:1;                               /*!< */
		unsigned	i8ns:3;                                 /*!< */
		unsigned	i8pf:1;                                 /*!< */
		unsigned	i8nr:3;                                 /*!< */
	}ctli_8;                                                /*!< */
	/*!
	 *  @brief   Control Field
	 *  @note    Control Field
	 */
	struct CTL_SFRM8
	{                                                       /*!< */
		unsigned	s81s:1;                                 /*!< */
		unsigned	s82s:1;                                 /*!< */
		unsigned	s83s:2;                                 /*!< */
		unsigned	s8pf:1;                                 /*!< */
		unsigned	s8nr:3;                                 /*!< */
	}ctls_8;                                                /*!< */
	/*!
	 *  @brief   Control Field
	 *  @note    Control Field
	 */
	struct CTL_MS8
	{                                                       /*!< */
		unsigned	ms81s:4;                                /*!< */
		unsigned	ms8pf:1;                                /*!< */
		unsigned	ms8nr:3;                                /*!< */
	}ctlms_8;                                               /*!< */
	/*!
	 *  @brief   Control Field
	 *  @note    Control Field
	 */
	struct CTL_USFRM8
	{                                                       /*!< */
		unsigned	usctl1:2;                               /*!< */
		unsigned	usctl2:2;                               /*!< */
		unsigned	us8pf :1;                               /*!< */
		unsigned	usctl3:3;                               /*!< */
	}ctlus_8;                                               /*!< */
};

											  /*******************************/
											  /*   Control Field WORK        */
											  /*(Field SetのBit合わせに使用) */
											  /*******************************/
/*!
 *  @brief   Control Field WORK(Field SetのBit合わせに使用)
 *  @note    Control Field WORK(Field SetのBit合わせに使用)
 */
union CTL_WK
{
	UCHAR				work_all;                           /*!< */
	/*!
	 *  @brief   
	 *  @note    
	 */
	struct	WK_8                                            /*!< Modulo  8 �?    */
	{                                                       /*!< */
		unsigned		field3:3;                           /*!< */
		unsigned		dummy2:5;                           /*!< */
	}wk_8;                                                  /*!< */
	/*!
	 *  @brief   
	 *  @note    
	 */
	struct	OTHERS
	{                                                       /*!< */
		unsigned		field1:1;                           /*!< */
		unsigned		dummy3:7;                           /*!< */
	}others;                                                /*!< */
};

#else

											  /*******************************/
											  /*   FRMRデー�? 格納テーブル  */
											  /*******************************/

/*!
 *  @brief   Control Field WORK(Field SetのBit合わせに使用)
 *  @note    Control Field WORK(Field SetのBit合わせに使用)
 */
union	FMR1_DAT		
{			
	struct	FMR1_INF                                        /*!< */
	{                                                       /*!< */
		unsigned	rej_ctl:8;                              /*!< */
		unsigned	vr:3;                                   /*!< */
		unsigned	n:1;                                    /*!< */
		unsigned	vs:3;                                   /*!< */
		unsigned	zero:1;                                 /*!< */
		unsigned	zero2:4;                                /*!< */
		unsigned	z:1;                                    /*!< */
		unsigned	y:1;                                    /*!< */
		unsigned	x:1;                                    /*!< */
		unsigned	w:1;                                    /*!< */
		unsigned	dummy:8;                                /*!< */
	}fmr1_inf;		
	UCHAR	fmr1[4];	
};


											  /*******************************/
											  /*        Control Field 1      */
											  /*******************************/
/*!
 *  @brief   Control Field WORK(Field SetのBit合わせに使用)
 *  @note    Control Field WORK(Field SetのBit合わせに使用)
 */
union CTL_FLD1				
{				
	UCHAR			ctl1_all;                               /*!< */
	struct CTL_IFRM8                                        /*!< */
	{                                                       /*!< */
		unsigned	i8nr:3;                                 /*!< */
		unsigned	i8pf:1;                                 /*!< */
		unsigned	i8ns:3;                                 /*!< */
		unsigned	i8zero:1;                               /*!< */
	}ctli_8;			
	struct CTL_SFRM8			
	{			
		unsigned	s8nr:3;	
		unsigned	s8pf:1;	
		unsigned	s83s:2;	
		unsigned	s82s:1;	
		unsigned	s81s:1;	
	}ctls_8;			
	struct CTL_MS8		
	{		
		unsigned	ms8nr:3;
		unsigned	ms8pf:1;
		unsigned	ms81s:4;
	}ctlms_8;		
	struct CTL_USFRM8		
	{		
		unsigned	usctl3:3;
		unsigned	us8pf :1;
		unsigned	usctl2:2;
		unsigned	usctl1:2;
	}ctlus_8;		
};

											  /*******************************/
											  /*   Control Field WORK        */
											  /*(Field SetのBit合わせに使用) */
											  /*******************************/

/*!
 *  @brief   Control Field WORK(Field SetのBit合わせに使用)
 *  @note    Control Field WORK(Field SetのBit合わせに使用)
 */
union CTL_WK						
{						
	UCHAR				work_all;                           /*!< */
	struct	WK_8                                            /*!< */
	{                                                       /*!< */
		unsigned		dummy2:5;                           /*!< */
		unsigned		field3:3;                           /*!< */
	}wk_8;					
	struct	OTHERS				
	{					
		unsigned		dummy3:7;		
		unsigned		field1:1;		
	}others;					
};						


#endif

											  /*******************************/
											  /*     L2処理�?テーブル       */
											  /*******************************/
/*!
 *  @brief    L2処理�?テーブル
 *  @note     L2処理�?テーブル
 */
struct LAP_L2
{
	UCHAR			padding1;                               /*!< */
	UCHAR			padding2;                               /*!< */
	UCHAR			address;                                /*!< 自局/他局アドレス格納エリ� */
	UCHAR			l2_cntl1;                               /*!< Contorol Field       */
};

/*!
 *  @brief   CPB common header
 *  @note    CPB common header
 */
typedef struct
{
	CMT_TSKIF_HEAD	 head;                                  /*!< */
	USHORT			link_num;                               /*!< */
	UCHAR			dummy1;                                 /*!< */
	UCHAR			dummy2;                                 /*!< */
}LBT_TSKIF_LPBCOM;

/*!
 *  @brief   CPRI common header
 *  @note    CPRI common header
 */
typedef struct LBT_TSKIF_CPRICOM_T
{
	CMT_TSKIF_HEAD		head;                               /*!< */
	USHORT				link_num;                           /*!< CPRI番号					*/
	UCHAR				dummy1;                             /*!< ダミー領域	1			*/
	UCHAR				dummy2;                             /*!< ダミー領域	2			*/
// 擬似装置 20210310
//	struct LBT_TSKIF_CPRICOM_T    *i_link;                  /*!< I queue pointer			*/
	UINT							i_link;                  /*!< I queue pointer			*/
// 擬似装置 20210310
	UINT					length;                         /*!< buffer length			*/
	UINT					disp;                           /*!< buffer offset sizes		*/
	UCHAR				Padding1;                           /*!< Padding1					*/
	UCHAR				Padding2;                           /*!< Padding2					*/
	UCHAR				Address;                            /*!< HDLC frame address area	*/
	UCHAR				Control;                            /*!< HDLC frame control area	*/
}LBT_TSKIF_CPRICOM;	

											  /*******************************/
											  /*    Layer 2制御用バッフ�?   */
											  /*    (SABM/UA/RR等を送出する  */
											  /*      際に使用)              */
											  /*******************************/
/*!
 *  @brief   Buffer デー�?アクセス用構造体
 *  @note    Layer 2制御用バッフ�?SABM/UA/RR等を送出する際に使用)
 */
struct LAP_LAYER2_BUF
{
	CMT_TSKIF_HEAD		head;                               /*!< */
	USHORT				link_num;                           /*!< CPRI番号					*/
	UCHAR				dummy1;                             /*!< ダミー領域	1			*/
	UCHAR				dummy2;                             /*!< ダミー領域	2			*/
// 擬似装置 20210310
//	LBT_TSKIF_CPRICOM    *i_link;                           /*!< I queue pointer			*/
	UINT				i_link;                           /*!< I queue pointer			*/
// 擬似装置 20210310
	UINT					length;                         /*!< buffer length			*/
	UINT					disp;                           /*!< buffer offset sizes		*/
	struct LAP_L2			l2_dt;                          /*!< Layer 2 デー�?   */
};


											  /*******************************/
											  /*  Buffer デー�?アクセス用構造体   */
											  /*******************************/
/*!
 *  @brief   送信キューポイン�? *  @note    送信キューポイン�? */
struct LAP_DATA
{
	UCHAR				data[200];                          /*!< */
};

											  /*******************************/
											  /*      送信キューポイン�?    */
											  /*******************************/
/*!
 *  @brief   送信キューポイン�? *  @note    送信キューポイン�? */
struct LAP_TX_QU
{
	LBT_TSKIF_CPRICOM		*qu_buf_p;                      /*!< */
	UCHAR				qu_ctl;                             /*!< */
};



											  /*******************************/
											  /*     バッファポインタ        */
											  /*******************************/
/*!
 *  @brief   バッファポインタ
 *  @note    バッファポインタ
 */
union	LAP_BUFADR 
{
	LBT_TSKIF_CPRICOM			*lap_buf_p;                 /*!< */
	struct LAP_L2			*lap_l2_p;                      /*!< */
	struct LAP_LAYER2_BUF	*lap_layer_p;                   /*!< */
	struct LAP_DATA			*lap_dt_p;                      /*!< */
	UINT		 disp ;                                     /*!< */
};


											  /*******************************/
											  /*     LAPB データ格納エリア   */
											  /*******************************/
/*!
 *  @brief   LAPB データ格納エリア
 *  @note    LAPB データ格納エリア
 */
struct LAPB_DAT	   
{
	LBT_TSKIF_CPRICOM		*i_head_p;                      /*!< */
	LBT_TSKIF_CPRICOM		*i_tail_p;                      /*!< */
	LBT_TSKIF_CPRICOM		*i_head2_p;                     /*!< */
	LBT_TSKIF_CPRICOM		*s_head_p;                      /*!< */
	LBT_TSKIF_CPRICOM		*s_tail_p;                      /*!< */
	LBT_TSKIF_CPRICOM		*u_head_p;                      /*!< */
	LBT_TSKIF_CPRICOM		*u_tail_p;                      /*!< */
	LBT_TSKIF_CPRICOM		*tx_head_p;                     /*!< */
	LBT_TSKIF_CPRICOM		*tx_tail_p;                     /*!< */
	UCHAR		status;                                     /*!< */
	UCHAR		vr;                                         /*!< */
	UCHAR		vs;                                         /*!< */
	UCHAR		va;                                         /*!< */
	UCHAR		l3int_flg;                                  /*!< */
	UCHAR		rej_cond;                                   /*!< */
	UCHAR		per_bsy;                                    /*!< */
	UCHAR		own_bsy;                                    /*!< */
	UCHAR		retry_cnt;                                  /*!< */
	UCHAR		dsc_iq_flg;                                 /*!< */
	UCHAR		l1bsy_flg;                                  /*!< */
	UCHAR		t1_flg;                                     /*!< */
	UCHAR		t2_cnt;                                     /*!< */
	UCHAR		t3_flg;                                     /*!< */
	UCHAR		t4_flg;                                     /*!< */
	UCHAR		n2;                                         /*!< */
	UINT			t1;                                     /*!< */
	UINT			t2;                                     /*!< */
	UINT			t3;                                     /*!< */
	UINT			t4;                                     /*!< */
	UINT			l2bsytmrval;                            /*!< */
	UINT			l1bsytmrval;                            /*!< */
	UINT			n1;                                     /*!< */
	UCHAR		own_adr;                                    /*!< */
	UCHAR		per_adr;                                    /*!< */
	UCHAR		ownk;                                       /*!< */
	UCHAR		peerk;                                      /*!< */
	UCHAR		l2bsytmrflg;                                /*!< */
	UCHAR		buftyp1_bsyflg;                             /*!< */
	UCHAR		buftyp2_bsyflg;                             /*!< */
	UCHAR		buftyp3_bsyflg;                             /*!< */
	UCHAR		buftyp4_bsyflg;                             /*!< */
	UCHAR		dummy[3];                                   /*!< */
};


											  /*******************************/
											  /*     LAPB スタックテーブル   */
											  /*   (かっこ中はCHILL時の名称) */
											  /*******************************/
/*!
 *  @brief    LAPB スタックテーブル
 *  @note    かっこ中はCHILL時の名称
 */
struct  LBT_MNGTBL
{
	struct	LAPB_DAT	lap_dt;                             /*!< LAPB データ格納エリア */
	union	LAP_BUFADR	tr_adr;                             /*!< Buf Pointer(メイル用)    */
	union	LAP_BUFADR	buf_adr;                            /*!< Buf Pointer (urxref)  */
	union	LAP_BUFADR	tx_buf_adr;                         /*!< Buf Pointer1(utxref)  */
	union	LAP_BUFADR	layer2_adr;                         /*!< Buf Pointer2(utx2ref) */
	union	LAP_BUFADR	xid_adr;                            /*!< Buf Pointer3(utx3ref) */
	union	LAP_BUFADR	hnt_adr;                            /*!< Hunt�?Buf Pointer    */
	struct	LAP_TX_QU	tx_qu;                              /*!< */
	USHORT				cmd_rsp_flg;                        /*!< コマント? レスポン�?識別フラ� */
	USHORT				ctl_sv;                             /*!< Frame 種別格納エリ�?*/
	UCHAR				nr_sav;                             /*!< N(R)セーブエリア  */
	UCHAR				ns_sav;                             /*!< N(S)セーブエリア  */
	UCHAR				pf_sav;                             /*!< Poll/Final Bit セーブエ�?*/
	UCHAR				cmd_rsp_wk;                         /*!< コマント?レスポン�? ワークエリア */
	UCHAR				pf_wk;                              /*!< Poll/Filnal Bitワークエリア */
	UCHAR				adr_wk;                             /*!< アド�?ワークエリア      */
	UCHAR				vs_sav;                             /*!< V(S)セーブエリア  */
	UCHAR				vr_sav;                             /*!< V(R)セーブエリア  */
	UCHAR				cond;                               /*!< return 結果セー�?  */
	UCHAR				rcv_sbm_flg;                        /*!< SABM 受信フラ�?   */
	USHORT				link_num;                           /*!< 回線番号セーブエリア */
	UINT					t1_id;                          /*!< タイ�? IDセー�?   */
	UINT					t2_id;                          /*!< タイ�? IDセー�?   */
	UINT					t3_id;                          /*!< タイ�? IDセー�?   */
	UINT					t4_id;                          /*!< タイ�? IDセー�?   */
	UINT					l2bsydettmr_id;                 /*!< Layer2 busy detectタイ�?IDセー�?   */
	UINT					l1bsydettmr_id;                 /*!< Layer1 busy detectタイ�?IDセー�?   */
	UINT					l1bsydettmr_id2;
	UINT					l1bsydettmr_osid;
	UINT					leng_wk;                        /*!< Length ワー�?   */
	union	CTL_FLD1		ctl1_sav;                       /*!< Contorol Field1 セー� */
	union	FMR1_DAT	fmr1_sav;                           /*!< FRMRデータ格納エリア */
/*	UINT					buftyp_sav;		*/			/*  Buffer type格納エリ�? */
	UINT					iframecnt;                      /*!< I frame count	*/
	UINT					nsRejSnd;                       /*!< When send REJ frame,received NS num*/
};

/**
 * @brief   LAPB log
 */
typedef struct
{
	USHORT		loglable ;		
	USHORT		event_no ;			/*	起動要因番号		*/
	USHORT		year ;				/*	書込発生�?		*/
	UCHAR		month ;				/*	書込発生�?		*/
	UCHAR		day ;				/*	書込発生�?		*/
	UCHAR		hour ;				/*	書込発生�?		*/
	UCHAR		min ;				/*	書込発生�?		*/
	UCHAR		sec ;				/*	書込発生�?		*/
	UCHAR		msec ;				/*	書込発生ms			*/
	UINT 		regadr;				/* register address */
	UINT		regdat;				/* register data */	
}LPBW_LOG_T;

/**
 * @brief   LAPB link status
 */
typedef struct
{
	T_RRH_SYSTEM_TIME	datetime;                                   /*!< datetime */
	USHORT		setflg;
	USHORT		linkstate_prev;
	USHORT		linkstate_now;
	USHORT		linkevent;
	
	CHAR		func[32];
	UCHAR		lpbdat_prev[48];
	UCHAR		lpbmng_prev[48];
	UCHAR		lpbdat_now[48];
	UCHAR		lpbmng_now[48];
}LPBW_LINKSTAT;

/**
 * @brief   LAPB link status log table
 */
typedef struct
{
	UINT	   index;
	LPBW_LINKSTAT tbl[128];
}LPBW_LINKSTAT_LOGTBL;

#endif/*LB_TYP*/





