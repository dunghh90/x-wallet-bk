/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rsv_ftyp.h
 *  @brief	RE Supervision common function prototype definition
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_RSV_FTYP
#define		F_RSV_FTYP
#if 0
/** @addtogroup RRH_PF_RSV
* @{ */
/* ======================================================================== */
/*	RE監視タスク内共通関数プロトタイプ定義									*/
/* ======================================================================== */
extern	VOID	f_rsv_cm_L3Discon( USHORT, UINT );
extern	VOID	f_rsv_cm_TrxSetRlsNtc( UINT, USHORT, USHORT, USHORT );
#endif
/* ======================================================================== */
/*	RE監視タスク関数プロトタイプ定義										*/
/* ======================================================================== */

extern	VOID f_rsv_Main();
extern	VOID f_rsv_anlz( VOID * );
extern	VOID f_rsv_InitReq( VOID );
extern	VOID f_rsv_InitNtc( VOID );
extern	VOID f_rsv_AlmInfChk();
extern	VOID f_rsv_TraStaChgNtc( T_SYS_TRA_IRQNTC* );

extern VOID f_rsv_sub_actOnProc();
extern VOID f_rsv_sub_useOnProc();
extern VOID f_rsv_sub_useOffProc();
extern UINT f_rsv_sub_almStaChk(T_API_SVP_CARDSTA_INFO*);
extern UINT f_rsv_sub_actStaChk(T_API_SVP_CARDSTA_INFO*);
extern UINT f_rsv_sub_useStaChk(T_API_SVP_CARDSTA_INFO*);
extern VOID f_rsv_sub_errStaChk(T_API_SVP_CARDSTA_INFO*);
extern VOID f_rsv_sub_sndStaNtc(T_API_SVP_CARDSTACHG_NTC*);
extern VOID f_rsv_svLogSet(UINT*);

extern VOID f_rsv_SubscrStaReq(T_API_SVP_SUBSCARDSTA_REQ* );
extern VOID f_rsv_unSubscrStaReq(T_API_SVP_UNSUBSCARDSTA_IND* );
extern VOID f_rsv_TimeSet(T_API_SVP_SETTIME_REQ*  );
extern VOID f_rsv_CardReset(T_API_SVP_RESET_REQ *  );
extern VOID f_rsv_AllReL1Reset(T_API_SVP_ALLRE_L1RESET_IND *);
extern VOID f_rsv_ReL1Reset(T_API_SVP_RE_L1RESET_IND *);
extern VOID f_rsv_LedSet( T_API_SVP_LEDSET_NTC*  );
extern VOID f_rsv_ErrSet( T_API_SVP_ERRSET_NTC*  );
extern VOID f_rsv_AlmCyclChk( );
extern UINT f_rsv_com_multiAlmChk();
extern USHORT f_rsv_cm_GetHardAlmFact(UINT);
extern VOID f_rsv_cm_GetAlmInf( USHORT , USHORT*  );
extern VOID f_rsv_cm_ErrRegSet( USHORT action_flg, UINT writeAdr, UINT writeDat );
extern VOID f_rsv_rptCurCardSta(UINT uiSrcPQueueID);
extern VOID f_rsv_SetWave(T_API_SVP_SET_WAVE_IND *ind);
extern VOID f_rsv_SetAxC(T_API_SVP_SET_AxC_IND *ind);

extern void f_rsv_dbg();

#endif	/* M_RS_FTYP */
