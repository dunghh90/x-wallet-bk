/*!
 * @skip  $ld:$
 * @file  m_dl_ftyp.h
 * @brief Function prototype definition for DLM thread.
 * @date  2008/07/30 FFCS)Wuh Create
 * @date   2008/12/26 FFCS)Wuh M-S3G-eNBPF-01078												
 *				           don't set branch1 if card type is LRE 1TX
 * @date   2009/01/15 FFCS)Wuh S-S3G-eNBPF-01150												
 *				           add sum for S3G bandwidth and 3G carrier frequency is not right
 * @date  2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 * @date  2009/04/13  FFCS)Wuh modify for S3G PRC CR-00038-005 (CPRI spec V1.07)
 * @date  2009/04/30  FFCS)Wuh modify for CR-00038-005
 *                      move the process of FLASH erasing to file data sending
 * @date  2009/04/30  FFCS)Wuh modify for S3G PRC 7th release function system parameter mechanism change
 *                      move m_dl_SysParaDSet to m_cm_SysParaDSet
 * @date  2015/10/29  TDIPS)sasaki Update for REC CNT-N/E切替問題対応(ファイル情報報告/運用中ファイル情報報告)
 * @date  2015/11/19  TDIPS)sasaki Update for REC CNT-N/E切替問題対応(ファームウェアファイル報告/運用中ファームウェアファイル報告)
 * @date  2015/11/20  TDIPS)sasaki Update for REC CNT-N/E切替問題対応(ROMテーブル変更漏れ修正)
 *
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2015
 */

/* 二重取込み防止 */
#ifndef		M_DL_FTYP
#define		M_DL_FTYP

/* ======================================================================== */
/*	ダウンロード管理タスク関数プロトタイプ定義								*/
/* ======================================================================== */
extern VOID 	m_dl_main( UINT , UINT , VOID*  );
extern VOID     m_dl_rec_main( VOID* aBufaddr_p, USHORT awCpriNo );
extern	VOID    m_dl_init(VOID* bufp, USHORT cpri_no) ;
extern	VOID    m_dl_init_sysp( ) ;
extern	VOID	m_dl_cm_CRFileInfRspSnd( USHORT, USHORT );
extern	VOID	m_dl_cm_CRUSFileInfRspSnd( USHORT );
extern	VOID	m_dl_cm_ChgBootFace( USHORT );
extern	VOID	m_dl_cm_CRDataEndRspSnd( USHORT, USHORT );
extern	VOID	m_dl_cm_CRDataEndRspSnd_S3G( USHORT, USHORT, USHORT );
extern	VOID	m_dl_cm_CRUSDataEndRspSnd( USHORT );
extern	VOID	m_dl_cm_CRFirmRspSnd( USHORT, USHORT, USHORT, USHORT );
extern  VOID    m_dl_RFTRXSetTimTO( VOID );
extern	UINT	m_dl_SysParaSet( USHORT );
extern	UINT	m_dl_FirmWareSet( USHORT,USHORT,USHORT );
extern  UINT    m_dl_FirmWareSet_3G( UINT, USHORT, UINT);
extern  UINT    m_dl_FirmWareSet_S3G( UINT, USHORT, UINT);
extern VOID		m_dl_cm_DlEnd( USHORT );
extern VOID		m_dl_cm_DlRetry( USHORT );
extern VOID		m_dl_cm_USDlRetry( VOID );
extern VOID		m_dl_cm_ErrCtl(UINT);
extern VOID    	m_dl_UFimPro(VOID*);
extern VOID    	m_dl_FimPro(VOID*, USHORT);
extern VOID    	m_dl_SysPro(VOID*, USHORT);
extern UINT    	m_dl_SysParaSet_3G( VOID );
extern UINT    	m_dl_SysParaSet_S3G( VOID );
extern VOID		m_dl_cm_FlashProSnd( USHORT, USHORT, USHORT );
extern VOID 	m_dl_cm_RunHisSet(UINT, UINT, UINT, UCHAR*);
extern VOID 	m_dl_logPrint(FILE *outf);

extern VOID m_dl_fileDatSndMsg_sndNtc(USHORT cpri_no, USHORT msgKind, UINT sgmtno, UINT datSize, UCHAR *offset);

extern VOID     m_dl_cm_CRUSFirmRspSnd( USHORT a_rsp_result, 
										USHORT a_mjr_usever, USHORT a_min_usever, USHORT a_mjr_staver, USHORT a_min_staver);

/*From here is FHM*/
/** Send 運用中ファームウェアファイル報告応答2*/
extern VOID 	m_dl_CRUSFirmFileResSnd( USHORT awRslt );
/** The function of sending message betheen process / thread */
extern INT 		l3_com_sendMsg( UINT awSrcTQueueID, UINT awDstPQueueID, UINT awDstTQueueID, VOID* apMsgBuff, UINT awMsgSize );
extern USHORT	m_dl_cm_chkfilname(VOID* a_file_name );
extern VOID		m_dl_rl_fwrep_rsp2(VOID* bufp, USHORT cpri_no);
extern VOID 	m_dl_rl_ufwrep_rsp2(VOID* bufp, USHORT cpri_no);
extern EC_DLM_MAIN_EVT m_dl_evt_get(USHORT awSysType);
extern EC_DLM_MAIN_STN m_dl_ownstn_get(USHORT awSysType);
extern USHORT m_dl_rec_ownstn_get(USHORT awSysType, USHORT cpri_no);

/*-------sheet 7.5 7.6---------------*/
/*--7.5.1 ... 7.5.18*/
//7.5.1 ファームウェアファイル報告要求(LTE)
extern VOID m_dl_FWFileInfoRequestSnd(VOID* bufp, USHORT cpri_no);
//7.5.1 ファームウェアファイル報告要求(3G)
extern VOID m_dl_FWFileInfoRequestSnd_3g(USHORT cpri_no);
//7-5-2 ファームウェアファイル報告応答受信
extern VOID m_dl_FWFileReportRspRcv(VOID* bufp, USHORT cpri_no);
extern VOID m_dl_FWFileReportRspRcv_3g(VOID* bufp, USHORT cpri_no);

//7-5-5 ファイル情報報告通知(LTE)
extern VOID m_dl_FileInfoReportNotice(USHORT lw_CPRI_no);
//7-5-6 ファイル情報報告応答受信
extern USHORT m_dl_REFileInfReportRspRcv(VOID* bufp, USHORT cpri_no);
//7-5-7 REファイルデータ読込応答受信
//extern USHORT m_dl_REFileDataRspRcv(VOID* bufp, USHORT cpri_no);
//7-5-8 ファイルデータ送信(LTE)
extern USHORT m_dl_SendFileData(USHORT cpri_no);
extern VOID m_dl_SendFileDataStart(USHORT cpri_no);
extern VOID m_dl_SendFileDataEnd(USHORT cpri_no);
//7-5-9 ファイルデータ送信完了報告応答受信(LTE)
extern VOID m_dl_REFileDataSndCompleteReportRspRcv(VOID* bufp, USHORT cpri_no);
extern VOID m_dl_ResumeWaitRE();
//7-5-10 REファイル更新なし
extern VOID m_dl_REFileNotUpdate(VOID* bufp, USHORT cpri_no, USHORT aw_res, USHORT sndcmpntc_flg);
//7-5-10 REファイル更新なし(3G)
extern VOID m_dl_REFileNotUpdate_3g(USHORT cpri_no, USHORT aw_res, USHORT sndcmpntc_flg);
//7-5-11 ファームウェアファイル報告確認T.O.
//extern USHORT m_dl_FWFileReportVerify(VOID* bufp, USHORT cpri_no);
//7-5-12 ファームウェアファイル情報報告確認T.O.
extern VOID m_dl_FWFileReportVerifyTO(VOID* bufp, USHORT cpri_no);
//7-5-13 ファームウェア送信完了確認T.O.
extern USHORT m_dl_FWSendCompleteVerifyTO(VOID* bufp, USHORT cpri_no);
//7-5-14 ファームウェアファイル報告応答NG
extern VOID m_dl_FWFileReportRespondNG(VOID* bufp, USHORT cpri_no);
extern VOID m_dl_FWFileReportRespondNG_3g(VOID* bufp, USHORT cpri_no);
//7-5-15 ファイル情報報告応答NG
extern VOID m_dl_FileInfoReportRespondNG(VOID* bufp, USHORT cpri_no);
//7-5-16 ファイルデータ送信完了報告応答NG
extern VOID m_dl_FileDataSendCopmleteReportRespondNG(VOID* bufp, USHORT cpri_no);
//7-5-17 配下RE CPRIリンク断
extern USHORT m_dl_RELinkDisconect(VOID* bufp, USHORT cpri_no, USHORT system_type);
//7-5-18 REファイルデータ読込中止応答受信
extern VOID m_dl_REFileDataReadStpRspRcv(VOID* bufp, USHORT cpri_no);
/*7.6 functions declare*/
//7-6-1 増設REダウンロード中の運用中ファイル更新(LTE)
extern VOID m_dl_UpdateExpandREDowloadingFile(VOID* bufp, USHORT cpri_no);
//7-6-2 運用中ファイル更新中のRE増設ダウンロード(LTE)
extern VOID m_dl_REFileUpdateStaReq(VOID* bufp, USHORT cpri_no);
extern VOID m_dl_reFileUpdateCompNtc(USHORT cpri_no, USHORT a_result);
extern VOID m_dl_FileInfRes_OK(USHORT cpri_no);
extern VOID m_dl_FileInfRes_NG(VOID* bufp, USHORT cpri_no);
extern VOID m_dl_fileDataTransRes_OK(USHORT cpri_no);
extern VOID m_dl_fileDataTransRes_NG(USHORT cpri_no);
extern VOID m_dl_reFileUpdateTermination();
extern VOID m_dl_reFileUpdateNone(VOID* bufp, USHORT cpri_no);



extern VOID m_dl_getCpriSys_from_timerid( USHORT, USHORT, USHORT*, USHORT* );

/* Main table */
extern VOID    m_dl_init(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_err(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_sysp_dl_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_sysp_dl_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_refw_dl_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_refw_dl_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwreprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwreprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_fw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwreprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwreprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_fw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_refw_start_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_refw_start_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_load_refw_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_load_refw_stop_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_save_fw_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_usave_fw_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fwsavecmp_waiting_fwsave_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_finf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_finf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_finf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_finf_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_sysp_finf_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_finf_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_syspsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_fwsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fwsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_fwsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_fsndcmp_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_syspsndcmp_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fwsndcmp_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fsndcmp_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_syspsndcmp_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_fwsndcmp_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspinfrsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsnd_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsndcmprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwreprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwreprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fwrep_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwrep_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_ufinf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufinf_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_ufwsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufwsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_ufwsnd(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_ufsndcmp_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_idle_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspinfrsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsnd_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsndcmprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwreprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwreprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwrep_rsp(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fsnd_itv_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_syspinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_syspinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_syspsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_syspsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_syspsnd_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fwrep_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwrep_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fwinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fwinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_fl_fwsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_fwsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_fwsnd_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_fwsnd_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwrep_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufwinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_ufwsnd_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufwsnd_cnf_to(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_sysp_proc(USHORT awSysType);

extern VOID    m_dl_rl_syspinfrsp_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsnd_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsndcmprsp_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwreprsp_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_finf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_finf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_finf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwreprsp_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no);

extern VOID    m_dl_rl_syspinfrsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsnd_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_syspsndcmprsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fwreprsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_finfrsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsnd_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_fsndcmprsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufwreprsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufinfrsp_waiting_ufinf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsnd_waiting_ufinf_ntc(VOID* bufp, USHORT cpri_no);
extern VOID    m_dl_rl_ufsndcmprsp_waiting_ufinf_ntc(VOID* bufp, USHORT cpri_no);


/* dlr_fhm_mtrxtbl */
extern    VOID    m_dl_fhm_main( VOID* aBufaddr_p, USHORT awCpriNo );
extern    VOID    m_dl_fhm_idle_sysp_dl_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_err(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_idle_refw_dl_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_nop(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsnd_waiting_fw_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsnd_waiting_fw_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_fwload_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_fwloadstop_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_syspinfrsp_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_syspsndcmprsp_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsnd_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_finf_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_fwinf_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_syspinfrsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_syspsndcmprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsnd_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_fwrep_rsp(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_syspinfrsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_syspsndcmprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwload_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsnd_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsnd_waiting_fsnd_itv_to(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_syspinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to_3g(USHORT cpri_no);
extern    VOID    m_dl_fhm_finfrsp_waiting_fwinf_cnf_to(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_fhm_fsndcmprsp_waiting_fwsndcmp_cnf_to(VOID* bufp, USHORT cpri_no);
extern    INT    m_dl_fhm_sysp_filedatasend_req(USHORT, USHORT, INT *);
extern    INT     m_dl_fhm_sysp_fileinfo_req(USHORT, USHORT);

extern    VOID    m_dl_fhm_sysp_fileinfo_timersta(USHORT, USHORT);
extern    VOID    m_dl_fhm_sysp_filedatasendfin_timersta(USHORT, USHORT);
/* dlr_rec_mtrxtbl */
extern    VOID    m_dl_rec_main( VOID* aBufaddr_p, USHORT awCpriNo );
extern    VOID    m_dl_rec_idle_ufwrep_req(VOID* bufp, USHORT cpri_no);
extern    VOID    m_dl_rec_idle_fwrep_req(VOID* bufp, USHORT cpri_no);

extern	  VOID 	  m_dl_UFileInfRes_OK(USHORT cpri_no);
extern    VOID	  m_dl_UFileInfRes_NG(VOID* bufp, USHORT cpri_no);

extern    VOID    m_dl_rl_sysp_dl_rsp(USHORT, USHORT, USHORT);

extern    VOID    m_dl_rec_err(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_sysp_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_sysp_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_sysp_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fwreprsp_waiting_refw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_refw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_fw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_refw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_ufwreprsp_waiting_refw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_ufinfrsp_waiting_refw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsnd_waiting_fw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsndcmprsp_waiting_refw_stop_req(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_sysp_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_sysp_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_sysp_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_fwreprsp_waiting_refw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_refw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_fw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_refw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_ufwreprsp_waiting_refw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_ufinfrsp_waiting_refw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsnd_waiting_fw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsndcmprsp_waiting_refw_stop_ind(VOID* , USHORT );
extern    VOID    m_dl_rec_idle_fupdate_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_idle_finf_ntc(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_sysp_finf_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_refw_finf_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_syspsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_fwsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_fwsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_fwsnd_waiting_fwsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_fwsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_idle_fsndcmp_ntc(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_syspsndcmp_ntc(VOID* , USHORT );
extern    VOID    m_dl_rec_fsnd_waiting_fwsndcmp_ntc(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_syspsndcmp_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_fwsndcmp_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_idle_fwrep_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fwreprsp_waiting_fwrep_req(VOID* , USHORT );
extern    VOID    m_dl_rec_fwreprsp_waiting_fwrep_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_ufinfrsp_waiting_refw_ufinf_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_ufinfrsp_waiting_ufwsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsnd_waiting_ufwsnd(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsnd_waiting_ufwsndcmp_ntc(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_idle_ufwrep_req(VOID* , USHORT );
extern    VOID    m_dl_rec_ufwreprsp_waiting_ufwrep_req(VOID* , USHORT );
extern    VOID    m_dl_rec_ufwreprsp_waiting_ufwrep_rsp(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_syspinf_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_syspsndcmp_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_fwreprsp_waiting_fwrep_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_finfrsp_waiting_fwinf_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_fsndcmprsp_waiting_fwsndcmp_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_ufwreprsp_waiting_ufwrep_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_ufinfrsp_waiting_ufwinf_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_cnf_to(VOID* , USHORT );
extern    VOID    m_dl_rec_ufsndcmprsp_waiting_ufwsnd(VOID* , USHORT );

extern    VOID    m_dl_rl_fsndcmprsp_waiting_sysp_proc( USHORT );

extern    UINT    m_dl_CheckValidSyspara();

#endif	/* M_DL_FTYP */
