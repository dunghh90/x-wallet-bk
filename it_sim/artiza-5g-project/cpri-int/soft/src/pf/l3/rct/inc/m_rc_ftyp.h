/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_rc_ftyp.h
 *  @brief	RE control task common function prototype definition
 *  @date   2008/07/29 FFCS)linlj create
 *  @date   2010/01/06 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2011/05/13 FJT)koshida modify for CeNB-F-073-008(F)(REにおける故障発生時の機能部状態報告)
 *  @date   2011/09/17 FJT)Tokunaga modify for M-S3G-eNBSYS-02497 TS099項対応
 *  @date   2011/09/23 FFCS)Linlj  modify for 省電力対応
 *  @date   2011/10/24 FJT)Tokunaga modify for  M-S3G-eNBPF-04078 対応
 *  @date   2012/07/11 FFCS)Xut Create for CeNB-F-083-028対応
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 *  @date   2015/08/07 TDIPS)maruyama 装置立ち明けの関数
 *  @date   2015/08/24 TDIPS)maruyama 装置立ち明けの関数の型変更
 *  @date   2015/08/26 TDIPS)maruyama 装置立ち明けの関数の変更
 *  @date   2015/09/02 TDIPS)maruyama 装置立ち明けの関数の追加
 *  @date   2015/10/11 TDIPS)maruyama 装置情報としてBDE接続のSFPモジュール取得(m_gt_FHM_spfmodule)追加
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *  @date   2015/11/13 TDIPS)sasaki IT2問処No.181対処
 *  @date   2015/11/14 TDIPS)sasaki IT2問処No.186対処
 *  @date   2016/03/03 TDIPS)enoki RE自律リセット抑止解除要求送信関数定義
 *  @date   2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 *  @date   2016/11/22 FJT)ohashi TILTリセットによるスロット情報初期化処理抑止
 *  @date   2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 *  @date   2021/01/14 M&C)Huan.dh intermediate device add m_rc_ReCardCntRes()
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (delete time-out functions)
 *  @date   2021/02/04 M&C)Huan.dh define function forward CARONOFF from RE response to REC
 *  @date   2021/02/09 M&C)Huan.dh add forward RePortSta to RE|REC
 *  @date   2021/02/23 M&C)Huan.dh update ReCardInfoRep for intermediate device 
 * 
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/******************************************************************************************************************************/

#ifndef M_RC_FTYP_HEAD
#define M_RC_FTYP_HEAD

#include "rrhApi_Svp.h"

/****************************************************************************/
/*																			*/
/*					RE制御タスク共通関数プロトタイプ定義					*/
/*																			*/
/****************************************************************************/
/* 自カードリセット実行関数(m_rc_cm_TraRstExec)							*/
extern VOID m_rc_cm_TraRstExec(UINT, UINT);

/* スロット番号範囲チェック関数(m_rc_cm_SlotNoChk)							*/
extern UINT m_rc_cm_SlotNoChk(UINT);
/* TD-SRE TRX setting S. */
/* TRA設定可能判定関数(m_rc_cm_BrSetPosJug)								*/
extern UCHAR m_rc_cm_BrSetPosJug(USHORT, USHORT, UCHAR*);

/* 2枚実装TRAチェック関数(m_rc_cm_2BrChk)									*/
extern UCHAR m_rc_cm_2BrChk(USHORT, UCHAR, UCHAR, UCHAR*);

/* TRX解放実施関数(m_rc_cm_TrxRelExec)										*/
extern UINT m_rc_cm_TrxRelExec_LTE(USHORT, UCHAR, USHORT);

/* 全キャリア解放関数(m_rc_cm_AllCarRel_S3G)									*/
extern UINT m_rc_cm_AllCarRel_S3G(UCHAR br_bitmap,USHORT carr_no);
/* TD-SRE TRX setting E. */

/****************************************************************************/
/*																			*/
/*					RE制御タスク関数プロトタイプ定義						*/
/*																			*/
/****************************************************************************/
/* 初期化関数(m_rc_In)														*/
extern VOID m_rc_Ini(VOID);

/* REカード制御実施関数(m_rc_ReCardCnt)										*/
extern VOID m_rc_ReCardCnt(CMT_TSKIF_CPRIRCV_RECARDCNTREQ*);

/* RE Card control in case receive response from re/rec */
extern VOID m_rc_ReCardCntRes(CMT_TSKIF_CPRIRCV_RECARDCNTRES*);  

/* REカード制御実施関数(m_rc_ReCardCntExec)									*/
extern USHORT m_rc_ReCardCntExec(CMT_TSKIF_CPRIRCV_RECARDCNTREQ*,
										CMT_TSKIF_CPRISND_RECARDCNTRES*, USHORT);

/* REカード制御条件チェック関数(m_rc_ReCardCntChk)							*/
extern USHORT m_rc_ReCardCntChk(CMT_TSKIF_CPRIRCV_RECARDCNTREQ*, USHORT);

extern USHORT m_rc_BrNoToBit(USHORT br_no);

extern USHORT m_rc_getMaxBrNum();

extern USHORT m_rc_getMaxBrBits();

extern USHORT m_rc_getAllSlpBit();

extern RCT_CARDFB_BR_CARALM_T* m_rc_carAlmSearch(USHORT alm_code);

/* UL/DL carrier alarm check for carrier status report */
extern VOID m_rc_TraCarAlmChk(	USHORT	car_type,	USHORT br_bits, USHORT*		alm_flg);

/* REキャリア状態報告関数 */
extern VOID m_rc_ReCarStaRep(VOID*);
/* REキャリア状態報告関数( S3G ) */
extern VOID m_rc_ReCarStaRep_S3G(VOID*);
/* REキャリア状態報告関数( 3G ) */
extern VOID m_rc_ReCarStaRep_3G(VOID*);

/* 2021/01/18 M&C)Tri.hn Update based on No.18 (sequence 3-27) - add start */
/* REキャリア状態報告関数( S3G ) */
extern VOID m_rc_ReCarStaRepRes_S3G(VOID*);
/* 2021/01/18 M&C)Tri.hn Update based on No.18 (sequence 3-27) - add end */

/* TRAキャリア番号設定 */
extern VOID m_rc_TraAlmInfoSet_S3G(RCT_CARSTS_MNG_T* sts_tbl);

/* TRA スロット番号設定処理*/
extern VOID m_rc_TraSlotNumSet(RCT_CARSTS_MNG_T* sts_tbl);

/*TRAキャリア構成設定 for S3G */
extern VOID m_rc_carEveryBrDataGet(RCT_CARSTS_MNG_T* sts_tbl, int car_no);

extern VOID m_rc_carEveryComGet(RCT_CARSTS_MNG_T* sts_tbl, int car_no);

extern VOID m_rc_carEveryComClr(RCT_CARSTS_MNG_T* sts_tbl, int car_no);

/* RE制御タスクメイン関数(m_rc_ReCntMain)	 								*/
extern VOID m_rc_ReCntMain( UINT, UINT, VOID*);

/* RE PORT制御関数(m_rc_RePortCnt)											*/
extern VOID m_rc_RePortCntReq(CMT_TSKIF_CPRIRCV_REPORTCNTREQ*);
extern VOID m_rc_RePortCntRes(USHORT, USHORT);

/* RE PORT状態報告関数(m_rc_RePortStaRep)									*/
extern VOID m_rc_RePortStaRep(VOID*);

/* RE PORT forward message to rec(m_rc_RePortStaRes_LTE)									*/
extern VOID m_rc_RePortStaRes_LTE(VOID* parm_p);

/* RE PORT forward message to re(m_rc_RePortStaReq_LTE)									*/
extern VOID m_rc_RePortStaReq_LTE(VOID* parm_p);

/* REリセット関数(m_rc_ReRst)												*/
extern VOID m_rc_ReRst(CMT_TSKIF_CPRIRCV_RERSTREQ*);

extern USHORT m_rc_GetReCardFbBits();

/* REスロット情報報告関数(m_rc_ReSlotRep)									*/
extern VOID m_rc_ReSlotRep(VOID*);

/* REスロット情報処理関数(m_rc_SlotInf_Rcv_Rsp)								*/
extern VOID m_rc_SlotInf_Rcv_Rsp( CMT_TSKIF_CPRISND_SLOTINFRES* parm_p  );

/* RE状態報告関数(m_rc_ReStaRep)											*/
extern VOID m_rc_ReStaRep(CMT_TSKIF_CPRIRCV_RESTSREQ* aparm_p);
extern VOID m_rc_ReStaRes(CMT_TSKIF_CPRIRCV_RESTSRES* aparm_p);

/* RE時刻設定関数(m_rc_ReTimSet)											*/
extern VOID m_rc_ReTimSet(CMT_TSKIF_CPRISND_RETIMSETREQ*);

/* Toffset値情報報告関数(m_rc_ToffsetRep)									*/
extern VOID m_rc_ToffsetRep(VOID*);

/* 無変調キャリア出力ON/OFF設定関数(m_rc_UnCarOptOnOffDes)									*/
extern VOID m_rc_UnCarOptOnOffDesReq(VOID*);
extern VOID m_rc_UnCarOptOnOffDes_3g_Res(CMT_TSKIF_CPRIRCV_CARONOFFREQ*, USHORT);
extern VOID m_rc_UnCarOptOnOffDes_lte_Res( CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ*, USHORT);
extern VOID m_rc_CarOnOff_Rcv_Rsp( CMT_TSKIF_CPRISND_CARONOFFRES_S3G* parm_p  );

/* TD-SRE TRX setting S. */
/* TRX set */
extern VOID m_rc_TrxSet_LTE(CMT_TSKIF_CPRIRCV_TRXSET_S3G_REQ *parm_p);
/* TD-SRE TRX setting E. */

/* TRX解放関数(m_rc_TrxRel_S3G)													*/
extern VOID m_rc_TrxRel_S3G(CMT_TSKIF_CPRIRCV_TRXRELREQ*);

/* CPRIリンク断発生時の自律TRX解放													*/
extern VOID m_rc_CPRIdis_TrxRel(VOID*);

/* TRX解放通知受信関数(m_rc_TrxRelNtcRcv)									*/
extern VOID m_rc_TrxRelNtcRcv(CMT_TSKIF_TRXRLSNTC*);

/* TRX設定条件チェック関数(m_rc_TrxSetChk_S3G)									*/
extern UINT m_rc_TrxSetChk_S3G(CMT_TSKIF_CPRIRCV_TRXSET_S3G_REQ*);

/* カード名取得関数(m_rc_GetName)											*/
extern VOID m_rc_GetName(USHORT*, UCHAR*, UINT*);

/* 2021/02/18 M&C)Tri.hn - Update based on No67 - Sequence 3-30 - Modify start */
/* カード診断関数(m_rc_ReCardDia)											*/
extern VOID m_rc_ReCardDiaReq(CMT_TSKIF_CPRIRCV_RECRDDIAREQ*);
extern VOID m_rc_ReCardDiaRes( CMT_TSKIF_CPRIRCV_RECRDDIARES* );
extern VOID m_rc_ReCardDiaResSnd( USHORT, CMT_TSKIF_CPRIRCV_RECRDDIARES*);
/* 2021/02/18 M&C)Tri.hn - Update based on No67 - Sequence 3-30 - Modify end */

/* REカード状態報告関数(m_rc_ReCardStaRep_3G)	for 3G								*/
extern VOID m_rc_ReCardStaRep_3G(CMT_TSKIF_CPRIRCV_CARDSTSREQ*);

/* REカード状態報告関数(m_rc_ReCardStaRep_S3G)		for	S3G						*/
extern VOID m_rc_ReCardStaRep_S3G(CMT_TSKIF_CPRIRCV_CARDSTSREQ*);

/* RE card status report function for LTE operation mode */
extern VOID m_rc_ReCardStaRep_LTE(CMT_TSKIF_CPRIRCV_CARDSTSRES*);

/* Get カード基本監視頁E岼 */
extern VOID m_rc_GetSvitem( USHORT* , USHORT);

/* Get 機能部基本監視項目 */
extern VOID m_rc_GetSvitemfunc( USHORT* , USHORT);

/* Get カード基本監視頁E岼 */
extern VOID	m_rc_GetExtSvitem(USHORT* , USHORT );

/* Get カード基本監視頁E岼 for 3G */
extern VOID	m_rc_GetExtSvitem_3G(USHORT* , USHORT );

/*  Convert function kind to index */
extern USHORT	m_rc_ConvFuncKindToId( USHORT  );

/*  compare 配下REの機閨E部単位情報, get 機閨E部単位情報  */
extern VOID m_rc_GetFuncEvery( 	T_RRH_CARDST_S3G_FUNC_EVERY* ,
							T_RRH_CARDST_S3G_FUNC_EVERY,
							USHORT* );

/* Init table value for LTE */
extern VOID m_rc_GetInitValue_S3G(CARDSTS_INF_S3G*, USHORT*, USHORT, USHORT);
extern INT m_rc_IsSwap(VOID);

/* REカード状態報告条件チェック関数(m_rc_ReCardStaRepChk)					*/
extern USHORT m_rc_ReCardStaRepChk(CMT_TSKIF_CPRIRCV_CARDSTSREQ*);

/* REカード状態報告ALM/ERR情報設定(m_rc_CardStaALMERR_InfSet)				*/
extern VOID m_rc_CardStaALMERR_InfSet(USHORT, USHORT*, USHORT*, USHORT*, USHORT*, USHORT);

/* REカード状態報告ERR数カウント(m_rc_CardStaERR_Count)				*/
extern USHORT m_rc_CardStaERR_Count(USHORT);

/* REカード状態報告 S3G fb info set			*/
extern VOID m_rc_CardFB_InfSet(	USHORT, USHORT *, USHORT, USHORT, USHORT *, USHORT * );

/* REカード状態報告 S3G branch info set			*/
extern VOID m_rc_CardFB_BrInfSet(	USHORT, USHORT *, USHORT, USHORT, USHORT, USHORT, USHORT *, USHORT * );

/* REカード状態報告 S3G fb work table set	*/
extern VOID m_rc_CardFB_WorkTblSet(	USHORT, USHORT *, USHORT );

/* FTPデータ送信用PORT番号設定応答関数 */
/* 2021/25/01 M&C)Tri.hn Update based on No.23 (seq 3-23, 3-24) (modify start) */
extern VOID m_rc_FtpPortSetRes( CMT_CPRIF_FTPPORTSETRES* );
extern VOID m_rc_FtpPortSetResSnd( USHORT, USHORT );
/* 2021/25/01 M&C)Tri.hn Update based on No.23 (seq 3-23, 3-24) (modify end) */

/* REバージョン報告関数(m_rc_ReVerRep)										*/
extern VOID m_rc_ReVerRep(CMT_TSKIF_CPRIRCV_REVERREQ*);

/* REバージョン報告条件チェック関数(m_rc_VerRepChk)							*/
extern USHORT m_rc_VerRepChk(CMT_TSKIF_CPRIRCV_REVERREQ*);

/* 配下REからのREバージョン報告応答受信関数 */
extern VOID m_rc_ReVerRepRes( CMT_TSKIF_CPRIRCV_REVERRES* );

/* REバージョン報告応答送信関数 */
extern VOID m_rc_REVerRepRspSnd( USHORT, USHORT );

/* 2021/25/01 M&C)Tri.hn Delete based on No.23 (seq 3-23, 3-24) (delete start) */
/* FHMバージョン報告応答送信関数 */
// extern VOID m_rc_FHMVerRepRspSnd( USHORT, USHORT ,REVER_INF, UCHAR *, UINT);
/* 2021/25/01 M&C)Tri.hn Delete based on No.23 (seq 3-23, 3-24) (delete end) */

/* REC報告内容初期設定関数 */
extern VOID m_rc_REVerInitSet( INT, USHORT, USHORT );

/* 要求を送信したすべての配下REからの受信チェック関数 */
extern USHORT m_rc_ReVerRepRcvAllChk( USHORT adw_Sys_type);

/* 要求を送信したすべての配下REからの応答結果チェック関数 */
extern USHORT m_rc_ReVerRepResAllChk( USHORT );

/* 2021/25/01 M&C)Tri.hn Delete based on No.23 (seq 3-23, 3-24) (delete start) */
/* REバージョン報告確認タイマ タイムアウト関数 */
// extern VOID m_rc_ReVerRepTO( USHORT );
/* 2021/25/01 M&C)Tri.hn Delete based on No.23 (seq 3-23, 3-24) (delete end) */

/* バージョン情報設定関数 */
extern VOID m_rc_REVerInfoSet(USHORT , REVER_INF* );

/* ハードバージョン設定関数(m_rc_HardVerSet)								*/
extern VOID m_rc_HardVerSet(VOID*, REVER_INF*, UINT*);

/* ハードバージョン設定関数(m_rc_HardVerSet_card)							*/
extern VOID m_rc_HardVerSet_card(VOID*, REVER_INF*, UINT*);

/* ファームバージョン設定関数(m_rc_FarmVerSet)								*/
extern VOID m_rc_FirmVerSet(REVER_INF*);

/* ファームバージョンID情報取得関数(m_rc_FarmIdInfGet)						*/
extern VOID m_rc_FirmIdInfGet(REVER_INF*, UCHAR*, UINT*);

/* ハードバージョンID情報取得関数(m_rc_HardIdInfGet)						*/
extern VOID m_rc_HardIdInfGet(VOID*, UINT, REVER_INF*, UCHAR*, UINT*);

/* RE装置構成情報報告関数(m_rc_ReStrRep_3G)									*/
extern VOID m_rc_ReStrRep_3G(VOID*);

/* RE装置構成情報報告関数(m_rc_ReStrRep_S3G)									*/
extern VOID m_rc_ReStrRep_S3G(VOID*);

/*RE制御個別ログ収集処理 */
extern	VOID	m_rc_loging( UINT, UINT, VOID* );

extern	VOID m_rc_cm_AlmCodeCnv( T_API_SVP_ALMINFO  );
extern	VOID m_rc_TraStaChgNtc( T_API_SVP_CARDSTACHG_NTC* );

extern VOID m_rc_SubGetVerRsp(VOID*);

extern VOID m_rc_CpriStartup( VOID );

extern USHORT m_rc_ReCardCntChk_CntKind(CMT_TSKIF_CPRIRCV_RECARDCNTREQ *parm_p, USHORT sigsys_rslt);

extern VOID m_rc_SysParaUpd(CMT_TSKIF_SYSPARA_CHG_NTC *);

extern USHORT m_rc_HaveHardErrChk();
extern USHORT m_rc_HaveSoftErrChk();

extern VOID m_rc_TrxSetDoubleChk(USHORT carno);

extern USHORT m_rc_CardSta_SearchAlmBr(USHORT alm_code ,USHORT *alm_branch);

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* FLD-MTデータコマンド転送送信要求をチェック */
// extern UINT m_rc_Fld_CmdTransSndReqParamChk( CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ* parm_p );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* Send FLD-MTデータコマンド転送送信応答 */
extern VOID	m_rc_FldMt_DataCmdTransferSndRspSend( USHORT awResult, USHORT awSystem);

/** FLD-MTデータコマンド転送送信要求処理・*/
extern VOID m_rc_FldMt_DataCmdTransferSndReq( CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ* parm_p );

/**  FLD-MTデータコマンド転送送信応答処理・*/
extern VOID m_rc_FldMt_DataCmdTransferSndResRcv( CMT_TSKIF_CPRISND_FLDCOMSNDRES* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/** FLD-MTデータコマンド転送送信確認タイマタイムアウト処理*/
// extern VOID m_rc_FldMt_DataCmdTransferSndTO(  USHORT awTimerId );
/** FLD-MTデータファイル転送受信確認タイマタイムアウト */
// extern VOID m_rc_FldMt_FilTransTO( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* RE装置構成情報報告応答*/
extern VOID m_rc_ReStrRep( VOID* aparm_p);
extern VOID m_rc_ReStrRes( VOID* aparm_p);
/*(3G) the function that synthesize RE装置構成情報*/
extern UINT m_rc_GenResFromReSub3g();
/*(LTE) the function that synthesize RE装置構成情報*/
extern UINT m_rc_GenResFromReSubLte();
/*Get max value function*/
extern USHORT m_rc_GetMaxValue(USHORT awVal1, USHORT awVal2 );
/** 配下REからのSV-MT/FLD-MT接続強制解放応答受信処理*/
extern VOID m_rc_SvMt_ForReleaseConnectRes( CMT_TSKIF_CPRIRCV_SVMTFORRELRES* parm_p );
/** SV-MT/eNB-MT接続強制解放要求受信処理 */
extern VOID m_rc_SvMt_ForReleaseConnectReq( CMT_TSKIF_CPRIRCV_SVMTFORRELREQ* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/**  SV-MT/eNB-MT接続強制解放確認タイマ タイムアウト処理*/
// extern VOID m_rc_SvMt_ForReleaseConnectTO( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/** 要求を送信したすべての配下REから正常応答を受信チェック*/
extern USHORT m_rc_SvMt_ForReleaseConnectResRcvAllChk( USHORT adw_Sys_type);

/** eNB-MT接続強制解放応答送信処理 */
extern VOID m_rc_SvMt_ForReleaseRspSnd( USHORT awResult, USHORT awSystem );

/* recスレッドにTRX設定要求を受信し、転送する */
extern VOID m_rc_TrxSet_Transfer_Req( VOID* );

/* recスレッドにTRX設定要求を受信し、転送する(3G) */
extern VOID m_rc_TrxSet_Transfer_Req_3G( CMT_TSKIF_CPRIRCV_TRXSETREQ* );

/* recスレッドにTRX設定要求を受信し、転送する(LTE) */
extern VOID m_rc_TrxSet_Transfer_Req_LTE( VOID* );

/* rctスレッドのTRX設定確認タイマ タイムアウト */
extern VOID m_rc_TrxSet_Trans_TO(USHORT);

/* Check parameter */
extern UINT m_rc_TrxSet_Chk_Params(VOID*);

/* Receive response from re/rec in case TRX制御状態 = アイドル */
extern VOID m_rc_TrxSet_Rcv_Rsp(CMT_TSKIF_CPRISND_TRXSETRES*);

/* Receive response from re/rec in case TRX制御状態 = アイドル(3G) */
extern VOID m_rc_TrxSet_Rcv_Rsp_3G(CMT_TSKIF_CPRISND_TRXSETRES*);

/* Receive response from re/rec in case TRX制御状態 = アイドル(LTE) */
extern VOID m_rc_TrxSet_Rcv_Rsp_LTE(CMT_TSKIF_CPRISND_TRXSETRES*);

/* Send TRX設定応答 to REC */
extern VOID m_rc_TrxSet_Snd_Rsp(USHORT, USHORT);

/* Send TRX解放応答 to REC */
extern VOID  m_rc_TrxRel_Snd_Rsp( USHORT, USHORT);

/* Receive TRX解放応答 from re/rec in case TRX制御状態 = アイドル */
extern VOID m_rc_TrxRel_Rcv_Rsp_LTE( CMT_TSKIF_CPRISND_TRXRELRES* );

/* Receive TRX解放応答 from re/rec in case TRX制御状態 = アイドル */
extern VOID m_rc_TrxRel_Rcv_Rsp_3G( CMT_TSKIF_CPRISND_TRXRELRES* );

/* Receive TRX解放応答 from re/rec in case TRX制御状態 = アイドル */
extern VOID m_rc_TrxRel_Rcv_Rsp( CMT_TSKIF_CPRISND_TRXRELRES* );

/* rctスレッドのTRX解放確認タイマ タイムアウト */
extern VOID m_rc_TrxRel_Trans_TO( USHORT );

/* RECからのTRX解放要求受信 */
extern VOID m_rc_TrxRel_Transfer_Req( CMT_TSKIF_CPRIRCV_TRXRELREQ* );

/* Send TRX解放応答 to REC */
VOID m_rc_TrxRel_Snd_Rsp( USHORT , USHORT  );

/* FLD-MTデータコマンド転送受信処理 */
extern VOID m_rc_Fld_CmdRcvReq( CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ* parm_p );

/* FLD-MTデータコマンド転送受信応答処理 */
extern VOID m_rc_Fld_CmdRcvRes( CMT_TSKIF_CPRIRCV_FLDCOMRCVRES* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* FLD-MTデータコマンド転送受信要求信号をチェック */
// extern UINT m_rc_Fld_CmdRcvReqParamChk( CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ* parm_p );

/* FLD-MTデータコマンド転送受信応答確認タイムアウト処理 */
// extern VOID m_rc_Fld_CmdRcvResConfirmTo( UINT adwTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* FLD-MTデータファイル転送送信要求処理 */
extern VOID m_rc_Fld_FilSndReq( VOID* parm_p );

/* FLD-MTデータファイル転送送信応答処理 */
extern VOID m_rc_Fld_FilSndRes( CMT_TSKIF_CPRIRCV_FLDFILSNDRES* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* FLD-MTデータファイル転送送信確認タイムアウト処理 */
// extern VOID m_rc_Fld_FilSndResConfirmTo( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* FLD-MTデータファイル転送送信要求信号をチェック */
extern UINT m_rc_Fld_FilSndReqParamChk( USHORT signal_kind, USHORT sndbyte_num );

/* 外部装置データ送信要求処理 */
extern VOID m_rc_ExtDataSndReq( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p );

/* 非AISG信号受信処理 */
extern VOID m_rc_NoneAisgSignal( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p );

/* 外部装置データ送信要求信号をバッファリングする。 */
extern UINT m_rc_SetBufLnkDataSnd( USHORT awSystemType, USHORT awSignalType, CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* atBufferData_p );
/* バッファリング送信要求データ処理 */
extern VOID m_rc_GetBufLnkDataSnd( USHORT awSystemType);
/* 外部装置データ送信応答待ちタイムアウト */
extern VOID m_rc_ExtDataSndRspTO( UINT adwTimerId);

/* The function of sending respone message from l3/rct to re/rec for FLD-MTデータコマンド転送受信処理  */
extern INT m_rc_Fld_CmdRcvl3SendMsg( USHORT sigsys, USHORT result );

/* The function of sending respone message from l3/rct to REC for FLD-MTデータファイル転送送信処理 */
extern INT m_rc_Fld_FilSndL3SendMsg( USHORT sigsys, USHORT result );

extern VOID m_rc_ReFldDiscRspRcv( CMT_TSKIF_CPRISND_FLMTCNCRELRES* aparm_p);
extern VOID m_rc_ReFldMReDiscReqRcv( CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ* aparm_p);
extern VOID m_rc_ReFldDiscRspSnd( USHORT , USHORT );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
// extern VOID m_rc_ReFldReDiscReqTO(UINT adwTimerId);
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* FLD-MT接続要求処理 */
extern VOID m_rc_FldMt_ConnectReq( CMT_TSKIF_CPRIRCV_FLDMTCNCREQ* parm_p );

/*  MTアドレス設定応答処理*/
extern VOID m_rc_FldMt_ConnectResp( CMT_TSKIF_CPRIRCV_FLDMTCNCRES* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* FLD-MT接続確認タイマタイムアウト処理 */
// extern VOID m_rc_FldMt_ConnectTo( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/*FLD-MT接続強制解放応答送信処理*/
extern VOID m_rc_FldMt_ConnectResSnd( USHORT awResult, USHORT awSystem, USHORT awCpriLink );

/* SV-MT/eNB-MT接続要求処理 */
extern VOID m_rc_SvMt_ConnectReq( CMT_TSKIF_CPRIRCV_SVMTCNTREQ* parm_p );

/* SV-MT/eNB-MTアドレス設定応答処理*/
extern VOID m_rc_SvMt_ConnectResp( CMT_TSKIF_CPRIRCV_SVMTCNTRES* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* SV-MT/eNB-MT接続確認タイマタイムアウト処理 */
// extern VOID m_rc_SvMt_ConnectTo( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* SV-MT/eNB-MT接続強制解放応答送信処理 */
extern VOID m_rc_SvMt_ConnectResSnd( USHORT awResult, USHORT awSystem, USHORT awCpriLink );

/* SV-MT/eNB-MTデータ受信要求関数 */
extern VOID m_rc_SndSVMTDatRcvReq( CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ* parm_p );

/* SV-MT/eNB-MTデータ受信応答関数 */
extern VOID m_rc_RcvSVMTDatRcvRes( CMT_TSKIF_CPRIRCV_SVMTDATRCVRES*  rsp_p );

/* Send SV-MT/eNB-MTデータ受信応答 */
extern VOID m_rc_SVMTDatRcvResSnd( USHORT  awRslt, USHORT awSystem );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* SV-MT/eNB-MTデータ受信確認タイマタイムアウト */
// extern VOID m_rc_SVMTDatRcvTO( UINT  awTimerID );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* SV-MT/eNB-MT接続解放要求													*/
extern VOID m_rc_SndSVMTCntRelReq( CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ* );

/* SV-MT/eNB-MT接続解放応答													*/
extern VOID m_rc_RcvSVMTCntRelRes( CMT_TSKIF_CPRIRCV_SVMTCNTRELRES*);

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* SV-MT/eNB-MT接続解放確認タイマタイムアウト */
// extern VOID m_rc_SVMTCntRelTO( UINT  awTimerID );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* Send SV-MT/eNB-MT接続解放応答 */
extern VOID m_rc_SVMTCntRelResSnd( USHORT  awRslt, USHORT awSystem );

/* FLD-MT接続強制解放要求受信処理 */
extern VOID m_rc_Fld_Mt_ForReleaseConnectReq( CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ* parm_p );

/* 配下REからのFLD-MT接続強制解放応答受信処理 */
extern VOID m_rc_Fld_Mt_ForReleaseConnectRes( CMT_TSKIF_CPRIRCV_FLMTCMPRELRES* parm_p );

/* 要求を送信したすべての配下REから正常応答を受信チェック */
extern USHORT m_rc_Fld_Mt_ForReleaseConnectResRcvAllChk( UINT adw_Sys_type);

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/* FLD-MT接続強制解放確認タイマ タイムアウト処理 */
// extern VOID m_rc_Fld_Mt_ForReleaseConnectTO( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* FLD-MT接続強制解放処理のグローバル変数を初期化する。 */
extern VOID m_rc_Fld_Mt_ForReleaseConnectInit( USHORT awSystem );

/* FLD-MT接続強制解放応答送信処理 */
extern VOID m_rc_Fld_Mt_ForReleaseRspSnd( USHORT awResult, USHORT awSystem );

/*SV-MT/eNBデータ送信要求処理 */
extern VOID  m_rc_SvMt_DatSndReq( CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ* parm_p );

/* SV-MT/eNBデータ送信応答処理*/
extern VOID m_rc_SvMt_DatSndRes( CMT_TSKIF_CPRIRCV_SVMTDATSNDRES* parm_p );

/*SV-MT/eNB接続確認タイマタイムアウト処理 */
extern VOID m_rc_SvMt_ConnectTo( USHORT awTimerId );

/*RE状態を集約して (LTE)*/
extern VOID m_rc_ReStaLTESub();
/*RE状態を集約して (3G)*/
extern VOID m_rc_ReSta3GSub();

/* 外部装置データ送信応答処理 */
extern VOID m_rc_ExtDataSndRsp( CMT_TSKIF_CPRIRCV_EXTDATSNDRES* parm_p);

/* 信号送信対象REから応答結果を決定する */
extern VOID m_rc_ExtDataSndRspSubProc( USHORT awSystemType, USHORT awBufResult );

extern VOID m_rc_ExtDataSndRspSubProc_OnlyMyself( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p, USHORT awSystemType, USHORT awBufResult );

/*SV-MT/eNBデータ送信応答処理*/
extern VOID m_rc_SvMt_DatSndRes( CMT_TSKIF_CPRIRCV_SVMTDATSNDRES* parm_p );

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/*SV-MT/eNB-MTデータ送信確認タイマタイムアウト処理*/
// extern VOID m_rc_SvMt_DataSndTo( USHORT awTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/*SV-MT/eNBデータ送信応答送信処理*/
extern VOID m_rc_SvMt_DataSndResSend( USHORT awResult, USHORT awSystem );
/*  Check parameter of SV-MT/eNBデータ送信要求*/
extern USHORT m_rc_SvMt_DatSndReqParaCheck( CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ* parm_p, USHORT awSystemType );

/*  Send FLD-MTデータファイル転送受信応答*/
extern VOID m_rc_FldMt_FilTransResSnd( USHORT  awRslt, USHORT awSystem );

/* 外部装置データ送信要求 */
extern VOID m_rc_ExtDataSndReq( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ*  );
/* 状態要求 */
extern VOID m_rc_StatusReportCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* 局リセット要求 */
extern VOID m_rc_SystermResetCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* 配下REリセット要求 */
extern VOID m_rc_UnderReResetCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
extern VOID m_rc_UnderReResetRep( USHORT, USHORT );
/* 局ON/OFF要求 */
extern VOID m_rc_SystermOnOffCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* 配下REON/OFF要求 */
extern VOID m_rc_UnderReOnOffCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* ヘルスチェック要求 */
extern VOID m_rc_HealthChkCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* AISG信号 */
extern VOID m_rc_AisgSignal( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* 非AISG信号 */
extern VOID m_rc_NonAisgSignal( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* );
/* Send response message to REC*/
extern VOID m_rc_ReStrRepResSnd( UINT adwSysType );

/* Get Toffset value from share memory */
extern USHORT m_rc_GetToffset( USHORT awSystermType);
/* 遅延補正応答(Step10) */
extern VOID m_rc_DelayAdjStep10_Rsp(VOID *buff_adr);

/* 外部装置データ受信要求処理 */
extern VOID m_rc_ExtDataRcvReq( CMT_TSKIF_CPRIRCV_EXTDATRCVREQ* parm_p );

/* 外部装置データ受信応答受信 */
extern VOID m_rc_ExtDataRcvRsp( CMT_TSKIF_CPRIRCV_EXTDATRCVRES* parm_p );

/* 外部装置データ受信応答待ちタイムアウト */
extern VOID m_rc_ExtDataRcvRspTo( UINT adwTimerId );

/* 外部装置データ受信要求信号をバッファリングする。 */
extern UINT m_rc_SetBufLnkDataRcv( USHORT awSystemType, USHORT awProcessSta, CMT_TSKIF_CPRIRCV_EXTDATRCVREQ* atBufferData_p );

/* バッファリング受信要求データ処理 */
extern VOID m_rc_GetBufLnkDataRcv( USHORT awSystemType );

/* 外部装置データ受信要求送信処理 */
extern UINT m_rc_ExtDataRcvReqExe( CMT_TSKIF_CPRIRCV_EXTDATRCVREQ* parm_p, USHORT awProcessSta, USHORT awSystemType );

/* init table that save information of all RE (LTE) */
extern VOID m_rc_ReEqpStrInfInitTable_LTE();

/* init table that save information of all RE (3G) */
extern VOID m_rc_ReEqpStrInfInitTable_3G();

/* FLD-MTデータファイル転送受信応答 */
extern VOID m_rc_FldMt_FileTransRes(CMT_TSKIF_CPRIRCV_FLDFILRCVRES* );
/* FLD-MTデータファイル転送受信要求 */
extern VOID m_rc_FldMt_FileTransReq(void *);
/* タイムアウト発生通知受信処逅 */
extern VOID m_rc_TimeOutNtc( UINT );

/* Build 外部装置データ受信要求送信 */
extern VOID m_rc_ExtDataRcvBuildData(CMT_TSKIF_CPRIRCV_EXTDATSNDREQ*, CMT_TSKIF_CPRIRCV_EXTDATRCVREQ*, UCHAR, USHORT);

extern UCHAR m_rc_get_AlmB(UCHAR);

extern USHORT m_rc_get_AlmA();

/*Send MT address setting response function*/
extern VOID m_rc_MtAddrSetRspSnd(UINT systype,USHORT result);

// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete start */
/*MTアドレス設定確認タイマ」タイムアウト(REC)処理*/
// extern VOID m_rc_Mt_AddressSetTo( UINT adwTimerId );
// 2021/02/04 M&C)Tri.hn Update code based on spec no.68 - delete end */

/* MTアドレス設定応答処理 */
extern VOID m_rc_Mt_AddressSetRsp( CMT_TSKIF_CPRIRCV_MTADDSETRES* parm_p );

/*MTアドレス設定要求処理 */
extern VOID m_rc_Mt_AddressSetReq( CMT_TSKIF_CPRIRCV_MTADDSETREQ* parm_p );

/*FTPデータ送信用PORT番号設定処理 */
extern VOID m_rc_FtpPortSetReq( CMT_TSKIF_CPRISND_PORTSETREQ* buff_adr );

/*REカード診断要求処理 */
extern VOID m_rc_ReCardDiaReq( CMT_TSKIF_CPRIRCV_RECRDDIAREQ* buff_adr );

/******************
* REカード情報報告
********************/
/**/
/*REカード情報報告要求処理 */
extern VOID m_rc_ReCardInfoRepReq( VOID* parm_p );

/* REカード情報報告要求(3G) */
extern VOID m_rc_ReCardInfoRepReq_3g( CMT_TSKIF_CPRIRCV_CARDINFREQ* parm_p );

/* REカード情報報告要求(LTE) */
extern VOID m_rc_ReCardInfoRepReq_LTE( CMT_TSKIF_CPRIRCV_CARDINFREQ* parm_p );

/* REカード情報報告応答 */
extern VOID m_rc_ReCardInfoRepRsp( VOID *parm_p );

/* REカード情報報告応答(3G) */
extern VOID m_rc_ReCardInfoRepRsp_3g( CMT_TSKIF_CPRIRCV_CARDINFRES* parm_p );

/* REカード情報報告応答(3G)待ちタイムアウト */
extern VOID m_rc_ReCardInfoRepRspTimeout_3g( USHORT awTimerId );

/* REカード情報報告応答初期値設定 */
extern VOID m_rc_ReCardInfoRep_RspInit( CMT_CPRIF_CARDINFRES* atpRspMsg );

/* カード名決定処理 */
extern VOID m_rc_ReCardInfoRep_GetCardName( UCHAR* acpCardName );

/* REカード情報報告応答送信(3G) */
extern VOID m_rc_ReCardInfoRepRspSend( USHORT result );

/*RE card status report process*/
extern VOID m_rc_ReCardStaRep(CMT_TSKIF_CPRIRCV_CARDSTSREQ *parm_p);

/*  RE Disconnecion Request */
extern VOID m_rc_Cpri_Disconnect_Ntc(CMT_TSKIF_CPRIDISCONN_NTC* parm_p);

/* CPRI接続通知*/
extern VOID m_rc_ReConn_buffchk(VOID* buff_adr);

/* RE 強制停止 */
extern VOID m_rc_ReHCStop(USHORT linkno, USHORT system_type, T_RRH_RESTOP_FACTOR stopFactor);

/* RE立ち上げ要求 */
extern VOID m_rc_ReStr_Req(USHORT linkno, USHORT system_type);

/* RE立ち上げ応答 */
extern VOID m_rc_ReStr_Rsp(VOID* buff_adr);

/* RE立ち上げ応答(LTE) */
extern VOID m_rc_ReStr_Rsp_lte(CMT_TSKIF_RESTR_RSP* parm_p);

/*RE立ち上げ応答(3g) */
extern VOID m_rc_ReStr_Rsp_3g(CMT_TSKIF_RESTR_RSP* parm_p);

/* CPRI断通知 */
extern VOID m_rc_ReDis_buffchk(VOID* buff_adr);
extern VOID m_rc_RelFLDMT_CpriDwn(USHORT link_num, USHORT system_type);
extern VOID m_rc_RelSVMT_CpriDwn(USHORT link_num, USHORT system_type);
extern VOID m_rc_forceStopErrRel(USHORT link_num, USHORT system_type);

/* RE切断要求*/
extern VOID m_rc_ReDis_Req(USHORT linkno, USHORT layer_kind, USHORT system_type);

/* RE切断応答 */
extern VOID m_rc_ReDis_Rsp(VOID *buff_adr);

/* RE強制終了応答 */
extern VOID m_rc_ReForcestop_Ntc(VOID* buff_adr);

/* RE use status set request */
extern VOID m_rc_ReUseSet(T_API_CPRILINK_SET_REUSE_IND *parm_p);

/* REC起動完了までに変更されたRE接続有無状態の内容を反映する */
extern VOID m_rc_ReUseApplySettingsInBoot(VOID);

/* RE自律リセット抑止解除要求送信 */
extern VOID m_rc_ReSelfReset_PreventOff_Req_Send( USHORT linkno, USHORT reoff_flag );
	
/* CPRI番号REとLT共用 */
extern USHORT	rcw_cpri[D_RRH_CPRINO_RE_MAX * 2 - CMD_NUM1];

/* lteか3gか */
extern USHORT	rcw_system_type[D_RRH_CPRINO_RE_MAX * 2 - CMD_NUM1];	/* lteか3gか */

/* CPRI接続通知バッファカウンタ */
extern USHORT	rcw_buffcnt;

/* 装置立ち上げ	*/
extern	VOID m_rc_getStartUpRE();
extern	USHORT m_rc_get_startupREinfo(UINT re_no);
extern	VOID m_rc_StartUpRE_timerset();
extern  VOID m_rc_Step7_timerset(VOID);
extern 	VOID m_rc_SendMsg_reCpriLinkSat(UINT re_cnt, E_RRH_SFPLOG_TRIGGER trigger);
extern  VOID m_rc_SendMsg_reCpriLinkStp(USHORT link_num);
extern	VOID m_rc_CpriConnectNTC(VOID* buff_adr);
extern	VOID m_rc_infoAllRE(INT delayPrmChkEn);
extern	VOID m_rc_createEqpinfo();
extern	VOID m_rc_createREslotinfo();
extern	VOID m_rc_SendMsg_recCPRILinkSta();
extern	UINT m_rc_getConnectEqpInfoLTETbl(UINT	lp_cnt,UINT* firstElement_flg);
extern	UINT m_rc_getConnectEqpInfo3gTbl(UINT	lp_cnt,UINT* firstElement_flg);
extern  USHORT m_rc_RoundUp_Watt(USHORT powerunit, USHORT powerclass_org);
extern	VOID m_rc_chkDelay();
extern	UINT m_rc_reStartupRSP(VOID* buff_adr);

extern	VOID m_rc_NoRSP_HSTOP(USHORT *resync_tgt_re, T_RRH_CPRI_STATE *pCpriState);
extern	UINT m_rc_ReRstExe( USHORT  cpr_no );

extern	UINT m_gt_FHM_spfmodule( USHORT *fhm_spf );

extern void m_rc_dbg();

extern VOID m_rc_SetWave(VOID);

extern VOID m_rc_CpriRestartNtc(VOID);

#endif /* M_RC_FTYP_HEAD */

