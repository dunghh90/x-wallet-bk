/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_rc_ext.h
 *  @brief	RE control task external common table declaration
 *  @date   2008/07/29 FFCS)linlj create
 *  @date   2011/05/13 FJT)koshida modify for CeNB-F-073-008(F)(REにおける故障発生時の機能部状態報告)
 *  @date   2011/10/10 FFCS)Linlj  modify for 省電力対応
 *  @date   2012/03/15 ALP)Yamamoto 12A-01-006/M-S3G-eNBSYS-02601：SLC-RREにおけるALM/ERRコード処理変更について
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 *  @date   2015/08/07 TDIPS)maruyama 装置立ち明けの変数
 *  @date   2015/08/24 TDIPS)maruyama 装置立ち明けの変数の型変更、不要変数削除、変数追加
 *  @date   2015/08/26 TDIPS)maruyama 装置立ち明けの変数の変更
 *  @date   2015/09/10 TDIPS)maruyama get_re_EqpSlotInfo_＊＊をローカル変数に変更
 *  @date   2015/09/30 TDIPS)maruyama re_StartUp_CPRI_Conn_flg追加
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *  @date   2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 *  @date   2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/******************************************************************************************************************************/

extern UCHAR                            rcr_bcd_cvt_tbl[CMD_NUM16];
extern USHORT                           rcw_logcnt;
extern RCT_LOG_T                        rcw_log_tbl[RCD_LOG_MAX];
extern USHORT                           rcr_sverr_cvt_tbl[RCD_ERRCODE_SEAR_LEN][CMD_NUM3];
extern USHORT                           rcw_tskstn;
extern USHORT                           rcw_cpri_msg_flg;
extern USHORT                           rcr_cardFb_br_rscalm_tbl[RCD_FB_RESALM_NUM][CMD_NUM4];
extern RCT_CARDFB_BR_CARALM_T           rcr_cardFb_br_caralm_tbl[RCD_FB_CARALM_NUM];
extern RCT_RECARDSTAWORK_T              rcw_reCardStaWork_tbl[RCD_FB_NUM];
extern USHORT                           rcw_first_act_flg;
extern USHORT                           rcw_first_prc_act_flg;
extern USHORT                           rcw_remotecnt_flg;
extern USHORT                           rcw_device_kind;
extern USHORT                           rcr_pllalmcode[RCD_PLLALM_NUM];
extern USHORT                           rcr_cprialmcode[RCD_CPRIALM_NUM];
extern USHORT                           rcr_apdcomalmcode[RCD_APDALM_LOOP_NUM];
extern USHORT                           rcr_apd0almcode[RCD_APDALM_LOOP_NUM];
extern USHORT                           rcr_apd1almcode[RCD_APDALM_LOOP_NUM];
extern USHORT                           rcr_apd2almcode[RCD_APDALM_LOOP_NUM];
extern USHORT                           rcr_apd3almcode[RCD_APDALM_LOOP_NUM];
extern RCT_CARDFB_DEVKIND_T             rcr_cardFb_tdd_sre;
extern RCT_CARSTS_MNG_T                 rcw_carsts_tbl;
extern USHORT                           gw_rcw_SvEnb_Mt_ForRelConn_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_SvEnb_Mt_ForRelConn_ResSrcRcvSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_ReVerRep_ResSrcRcvSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_ReVerRep_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_SvEnbMt_ConnSts[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_ConnDst[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_ConnRel_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_ConnSts[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_RelReqRe[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_CmdRes_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_DataTranferRes_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_FilRes_WaitFlg[CMD_MAX_SYSNUM];
extern RCT_RCV_DATA_CMD_T               gw_rcw_FldMt_CmdRcvReqData[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern CPRIEQPSTR_INF_S3G               gt_recw_ReEqpStrInfTbl_LTE;
extern CPRIEQPSTR_INF                   gt_recw_ReEqpStrInfTbl_3G;
extern CMT_CPRIF_EQPSTRRES              gt_recw_ReEqpInfArr_3G[D_RRH_CPRINO_RE_MAX];
extern CMT_CPRIF_EQPSTRRES_S3G          gt_recw_ReEqpInfArr_LTE[D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_FLDMt_RcvRsp_WaitFlag[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_DataTranferRes_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_ExtSndReqSta[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SigSndReNo[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern RCT_DATA_CMD_T                   gt_rcw_DataTransInfo[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_DataTranferRes_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_ConnRes_WaitFlg[CMD_MAX_SYSNUM];
extern CMT_CPRIF_TRXSETREQ              gt_rcw_trxset_save_3g_tmp[CMD_MAX_CAR_3G];
extern CMT_CPRIF_TRXSETREQ_S3G          gt_rcw_trxset_save_lte_tmp[CMD_MAX_CAR_LTE];
extern USHORT							gt_rcw_trxset_datSize[CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_trxset_req_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_trxset_req_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_trxset_res_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_trxset_res_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_trxset_res_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_trxset_res_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_WaitResFlag_trxrel_3g[CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_WaitResFlag_trxrel_lte[CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_trxrel_req_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_trxrel_req_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_trxrel_res_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_trxrel_res_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE];
extern USHORT                           gw_rcw_trxrel_res_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G];
extern USHORT                           gw_rcw_trxrel_res_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE];
extern UINT                             gw_rcw_ReVerRep_res_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern UCHAR                            g_SvMt_retry_counter[CMD_MAX_SYSNUM];
extern UCHAR                            g_FldMt_retry_counter[CMD_MAX_SYSNUM];

/****************************************************************************/
/* テーブル初期化REバージョンREC報告内容 */
/****************************************************************************/
extern REVER_INF                        gw_rcw_ReVerRecRepCnt_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern USHORT                           gw_rcw_SvMt_CntRel_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvMt_ConnRelReqRe[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvMt_ConnRes_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvEnbMt_ConnSts[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvEnbMt_ConnDst[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_ConnDst[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_ConnSts[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvEnb_Mt_RelConn_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
extern RCT_EXT_DATA_SND_T               gt_rcw_ExtDataSndBuf[CMD_MAX_SYSNUM];
extern USHORT                           gw_recw_WaitFlag_MT_DatRcv[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvMt_DatRcvReqRe[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvMt_Conn_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvMt_DataSnd_WaitFlg[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_SvMt_DatRcvReqRe[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_FilTransReqRe[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_FldMt_FilTrans_WaitFlg[CMD_MAX_SYSNUM];
extern RCT_EXT_DATA_RCV_T               gt_rcw_ExtDataRcvBuf[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_ExtRcvReqSta[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_ExtRcvRspDestLinkNum[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_reRstSta[CMD_MAX_SYSNUM][DEV_NUM_MAX];
extern USHORT                           gw_rcw_tiltHcSta[CMD_MAX_SYSNUM];
extern USHORT                           gw_rcw_Mt_AdressSetReqSts[CMD_MAX_SYSNUM];
extern CMT_CPRIF_MTADDSETREQ            gwt_Mt_AdressSetInfo[CMD_MAX_SYSNUM];
extern USHORT                           inf_kind_val;
extern USHORT                           inf_slot_no;
extern RCT_CARD_INFO_DATA_T             rcw_CardInfoData_tbl;
extern USHORT                           rcw_cpri[D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1];
extern USHORT                           rcw_system_type[D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1];
extern UINT                             rcw_thread_qid[D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1];
extern USHORT                           rcw_buffcnt;
extern UINT                             re_StartUp_CPRI_Conn_flg;
extern UINT                             fhm_StartUp_flg;
extern UINT                             rec_Startup_First_flg[CMD_MAX_SYSNUM];
extern USHORT                           re_startup[D_RRH_CPRINO_NUM];
extern UINT                             re_startupREQ[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];
extern UINT                             re_startupRSP[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];

extern	const char *f_cpr_sfp[D_CPR_EEPROM_SFP_MAX];
extern	const int f_sfp_type[D_CPR_EEPROM_SFP_MAX];

extern USHORT							delay_adjust_step3;
extern USHORT							gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_MAXVAL+1];
extern T_RRH_REPORTSTA_S3G				*gw_rcw_shmRePortSta_logp;

extern	UINT	baseTim360;                                 /* 時刻 */
extern	UINT	re_funcFailValid[D_RRH_CPRINO_NUM];		/*	0:invalid, 1:valid, 2:step10exec	*/

