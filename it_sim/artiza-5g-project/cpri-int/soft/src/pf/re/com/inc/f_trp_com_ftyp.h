/*!
 *	@skip	$ld:$
 *	@file	f_trp_com_ftyp.h
 *	@brief	TRP common function prototype
 *	@date	2007/03/20	FFCS)linjunhua
 *  @date   2007/06/12  FJT)Nagasima M-S3G-eNBPF-00564
 *	@date	2008/07/29	FFCS)Tangj	S3G PRC eNB-008-002
 *	@date	2009/01/16	FFCS)Tangj	Add MTR timeout function
 *	@date   2011/02/10  FJT)Tokunaga CeNB-F-062-033(DCM) 運用パラメータの反映元と反映タイミング
 *	@date	2011/02/28	FJT)Tokunaga modify for M-S3G-eNBSYS-02253(TS111項【10B】)
 *  @date   2011/11/10 FJT)K.Koshida D-MICRO-008 3002形対応(LAPB削除,H/S inf変更対応)
 *
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2007-2011
 */
#ifndef _F_TRP_COM_FTYP_h
#define _F_TRP_COM_FTYP_h


///* =====================================================================*/
///* スレッドメイン関数 */
///* =====================================================================*/
//VOID	f_trp_trm_Fr_main();
//VOID	f_trp_tro_Fr_main();
//VOID	t_cr_main();
//VOID	f_trp_rec_Fcom_r_main();
//VOID	f_trp_red_F_r_main();
//VOID	t_er_Main();
//VOID	f_trp_err_Fr_main();
//VOID	t_mk_main();
//VOID	f_trp_lpb_Fcom_r_main();
//VOID	t_tx_Main();
//VOID	t_rx_Main();
//VOID	t_log_main();
//VOID	f_trp_mtr_Fr_main();
//
/* ======================================================================== */
/*	TRIF共通関数プロトタイプ定義											*/
/* ======================================================================== */

INT f_trp_com_Fregister_tim_M(bpf_ru_hrtm_notify_type_t,
        							UINT time,
       							bpf_ru_hrtm_key_inf_t key_inf,
        							UINT timerid,
        							INT* errcd,
        							UCHAR* file_p, UINT line);
#define f_trp_com_Fregister_tim(a, b, c, d, e) f_trp_com_Fregister_tim_M(a, b, c, d, e, ((UCHAR*)__FILE__), __LINE__ )

INT    f_trp_com_Fcancel_tim_M(UINT timerid, UCHAR* file_p, UINT line);
#define f_trp_com_Fcancel_tim(a) f_trp_com_Fcancel_tim_M(a, ((UCHAR*)__FILE__), __LINE__ )


#ifndef DEBUG_TRACE
INT    f_trp_com_Falloc_buf(UINT buf_size, VOID** buf_addr);
#else
INT    f_trp_com_Falloc_buf_M(UINT buf_size, VOID** buf_addr, UCHAR* file_p, UINT line);
#define f_trp_com_Falloc_buf(buf_size, buf_addr) f_trp_com_Falloc_buf_M(buf_size, buf_addr, ((UCHAR*)__FUNCTION__), __LINE__ )
#endif

#ifndef DEBUG_TRACE
VOID   f_trp_com_Ffree_buf(VOID* buf_addr);
#else
VOID   f_trp_com_Ffree_buf_M(VOID* buf_addr, UCHAR* file_p, UINT line);
#define f_trp_com_Ffree_buf(buf_addr) f_trp_com_Ffree_buf_M(buf_addr, ((UCHAR*)__FUNCTION__), __LINE__ )
#endif

/* Resister Reading(Single) Memory Access Type		*/
/* Reading access to Hardware resisters.This function include a stub for CT.*/
#ifndef DEBUG_TRACE
UINT   f_trp_com_Fread_reg(UINT regAdr, UINT* regDat);
#else
UINT   f_trp_com_Fread_reg_M(UINT regAdr, UINT* regDat, UCHAR* file_p, UINT line);
#define f_trp_com_Fread_reg(regAdr, regDat) f_trp_com_Fread_reg_M(regAdr, regDat, ((UCHAR*)__FUNCTION__), __LINE__ )
#endif

///* Resister Writing(Single) Memory Access Type		*/
///* Writing access to Hardware resisters.This function include a stub for CT.*/
//#ifndef DEBUG_TRACE
//UINT   f_trp_com_Fwrite_reg(UINT regAdr, UINT regDat);
//#else
//UINT   f_trp_com_Fwrite_reg_M(UINT regAdr, UINT regDat, UCHAR* file_p, UINT line);
//#define f_trp_com_Fwrite_reg(regAdr, regDat) f_trp_com_Fwrite_reg_M(regAdr, regDat, ((UCHAR*)__FUNCTION__), __LINE__ )
//#endif

INT    f_trp_com_Fget_tim(struct bpf_tm* tim);

//INT    f_trp_com_Fset_tim(struct bpf_tm* tim);
//
//UINT   f_trp_com_Fcpc_get_cprists(USHORT cpr_no);
//
//USHORT f_trp_com_Fcpc_get_actcpri_no(USHORT actlnk_gr);
//
///* Set CPRIMUX HDLC rate */
//UINT  f_trp_com_cprimux_hdlc_set(USHORT link_no);
//
//UINT  f_trp_com_cprimux_log_set(USHORT link_no, UINT giveReq);
//
///* writing CPRIMUX register */
//UINT    f_trp_com_w_otr_reg(UCHAR cardId, UCHAR regadr, USHORT regdat);
//
///* reading CPRIMUX register */
//UINT    f_trp_com_r_otr_reg(UCHAR cardId, UCHAR regadr, USHORT* regdat);
//
///* writing CPRIMUX local register */
//UINT  f_trp_com_r_mux_reg(UCHAR card_Id, USHORT	link_no, USHORT regadr, USHORT* regdat, UINT giveReq);
//
///* reading CPRIMUX local register */
//UINT  f_trp_com_w_mux_reg(UCHAR card_Id, USHORT	link_no, USHORT regadr, USHORT regdat, UINT giveReq);
//
//UINT f_trp_com_cprimux_reg_set(USHORT link_no, UINT giveReq);
//
//#if defined(OPT_CPRI_L2HARD)
//VOID f_trp_com_Fcmuxreg_set( VOID );
//#endif
//
//
//#define	TIMVAL_GET( timval ) 	(UINT)( timval * 10 )
//#endif /* #if 0 */
//
/*	メモリコピー処理	*/
INT f_trp_com_Fmemcpy(VOID	*dstAdr_p, const VOID *srcAdr_p, UINT cpyLen);
//
///*	メモリクリア処理	*/
//INT f_trp_com_Fmemclr(VOID* setAdr_p, UINT setLen);
//
//#if 0
INT   f_trp_com_Fget_precisetim(struct bpf_tm* tim, INT* ms);

//VOID f_trp_com_Fft_dmp(CHAR*, UINT, CHAR*);
VOID f_trp_com_Fhex_dmp(FILE*, CHAR*, UINT);
//VOID f_trp_com_Fhex_dmp2(CHAR*, UINT, UINT);
//
///* UINT f_trp_com_Flogout_res(UINT thr_q_num, UINT *msgnum); */
//
//USHORT f_trp_com_Factivelinkgr_get(USHORT cpr_no);
//
//VOID f_trp_com_FLog_switch( VOID );
//
//VOID f_trp_com_Frec_Obj_Init(VOID);
//
///****************************************************************************/
///* 共通部自身の関数一覧														*/
///****************************************************************************/
//
/*!< get the buffer type index according to the buffer size*/
//INT f_trp_com_Fget_buftyp(UINT buf_size, UINT *buf_type, UINT *buf_type_index );
//
/*!< send message to LAPB thread*/
//INT f_trp_com_Fbufbsy_send2lapb(INT msg_id, INT buf_type);
//
/*!< get thread internal number by thread id*/
//INT f_trp_com_Fgetthino_bythreadid(INT *thrino);
//
/*!< timeout entry function list*/
//VOID f_trp_com_Ftimeoutfor_cmc(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_cpc(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_edt(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_lpb(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_rec(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_red(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_rxs(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_trm(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_tro(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_txs(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//VOID f_trp_com_Ftimeoutfor_mtr(bpf_ru_hrtm_key_inf_t* key_inf, UINT timerId);
//
/*!< 共通関数Wrapper */
//INT f_trp_com_Fip_snd(VOID*);
//INT f_trp_com_Fit_snd(UINT, UINT, CHAR*, UINT, struct bpf_timespec*);
//
///*!< コピースレッド間メッセージ送信 */
//UINT f_trp_com_Fcpy_itm_snd(VOID*, UINT);
//
///* CPRI NO <--> Sector No */
//USHORT f_trp_com_Fsct_cpr_cnv(USHORT);
//USHORT f_trp_com_Fcpr_sct_cnv(USHORT);
//
/*!< size adjusting */
UINT f_trp_com_Fsize_adj(UINT, UINT);
//
///*!< 4バイトSWAP */
//VOID f_trp_com_F4bswap_ntoh(VOID*, UINT);
//VOID f_trp_com_F4bswap_hton(VOID*, UINT);
//
///*!< TCP conn/rel function list*/
//VOID f_trp_com_tcp_lnkon(INT linkkind);
//VOID f_trp_com_tcp_lnkoff(INT linkkind);
///*!< プロセス間メッセージ受信(通信スレッドに提供) */
//UINT f_trp_com_Fip_rcv(UINT, const VOID*);
//
///******************/
///* ログコマンド用 */
///******************/
//VOID f_trp_com_Flogout(INT*);				/* 受付 */
//VOID f_trp_com_Fdma_out(INT*);				/* 受付 */
//#ifdef DEBUG_TRACE_T
//VOID f_trp_com_regCont(INT*);				/* 受付 */
//VOID f_trp_com_intProc(INT*);				/* 受付 */
//VOID f_trp_com_Fdma_input(INT*);			/* 受付 */
//VOID f_trp_com_sndMsg(INT*);				/* 受付 */
//#endif
//VOID f_trp_com_FlogRunHisOut(INT*);			/* 受付 */
//VOID f_trp_txs_Ft_dmaout(INT*);				/* 受付 */
//VOID f_trp_rxs_Ft_dmaout(INT*);				/* 受付 */
//VOID f_trp_trm_Ft_sta_dsp(INT*, FILE*);		/* TRMステータス */
//VOID f_trp_edt_Ft_sta_dsp(INT*, FILE*);		/* EDTステータス */
extern VOID f_trp_rec_Fcom_t_sta_dsp(INT*, FILE*);	/* RECステータス */
//VOID f_trp_com_Flogout_sta(INT*, FILE*);	/* TCMステータス */
//VOID f_trp_com_Flogout_res(INT*, FILE*);	/* TCMリソース */
//VOID f_trp_red_F_t_sta_dsp(INT*, FILE*);	/* red log display */
//VOID f_trp_rxs_Ft_sta_dsp(INT*, FILE*);		/* RXS log display */
//VOID f_trp_txs_Ft_sta_dsp(INT*, FILE*);		/* TXS log display */
//VOID f_trp_lpb_Fcom_t_sta_dsp(INT*, FILE*);	/* LPBステータス */
//VOID f_trp_cmc_Fcom_t_sta_dsp(INT*, FILE*);	/* CMCステータス */
//VOID f_trp_cpc_Fcom_t_sta_dsp(INT*, FILE*);	/* CPCステータス */
//VOID f_trp_mtr_Ft_sta_dsp(INT*, FILE*);	/* MTRステータス */
//
///*!< log table register for Alarm log */
//VOID f_trp_com_FAlmLogTblReg(UCHAR	* tblName,UCHAR	* address,UINT	tblSize);
//
//VOID f_trp_com_Fl1log_rcvsnd(UINT linkno,UINT logindex,UINT logpos);
//VOID f_trp_com_RcvCounter(USHORT link_idx ,CMT_TSKIF_L1DATRCVNTC* rcv_p);
//VOID f_trp_com_SndCounter(USHORT link_idx ,CMT_TSKIF_L1DATIND* snd_p);
//
//VOID f_trp_com_FREfileDataSendTimeCanncel(USHORT cpri_no);
//
///* improve log */
//
#ifdef D_OPT_CPRING_LOG
//
//VOID f_trp_com_FInitImproveLog(VOID);
//VOID f_trp_com_FInitBufHntNgLog(VOID);
VOID f_trp_com_FGetBufHntNgLog(INT retVal, UINT bufType);
//VOID f_trp_com_FInitDmacIntNoClrLog(VOID);
//VOID f_trp_com_FGetDmacIntNoClrLog(USHORT lnkIdx);
//
//#define f_trp_com_FGetBufHntNgLogMng()						&(f_trp_tcm_WImprvLogMngTbl.bufHntNgLogMng)
//#define f_trp_com_FGetDmacIntNoClrLogMng(lnkIdx)			&(f_trp_tcm_WImprvLogMngTbl.dmacIntNoClrLogMng[lnkIdx])
//
#else
//
//#define f_trp_com_FInitImproveLog()							;
//#define f_trp_com_FInitBufHntNgLog()						;
#define f_trp_com_FGetBufHntNgLog(retVal, bufType)			;
//#define f_trp_com_FInitDmacIntNoClrLog()					;
//#define f_trp_com_FGetDmacIntNoClrLog(lnkIdx)				;
//
#endif

INT BPF_RU_UTTM_GETTIME(struct bpf_timeval *);
INT BPF_RU_UTTM_GETLOCALTIME( time_t* t, struct bpf_tm* tm, INT* ms );
#endif

