/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_ftyp.h
 *  @brief  CPRI management function prototype define
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2015/07/27 TDIPS)ikeda 引数修正
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

#ifndef    M_CR_FTYP
#define    M_CR_FTYP

extern VOID m_cr_main(UINT , UINT , UINT* );   

extern UINT m_cr_Anlz(UINT , UINT );

extern VOID m_cr_Init(UINT* buff_adr);

extern VOID m_cr_Nop(UINT* );
extern VOID m_cr_Nop2(UINT* );

extern VOID m_cr_CpriStateChgProc(UINT* );

extern VOID m_cr_CpriStateChgProcRE(UINT* );

extern VOID m_cr_DlinkSettingReq(USHORT linkno);

extern VOID m_cr_comLinkRecoverProc(UINT * );

extern VOID m_cr_ComLinkSettingL3Rsp(USHORT linkno, USHORT , USHORT );

extern VOID m_cr_ComLnkRegSet(USHORT , USHORT );

extern VOID m_cr_CpriMsgRcvProc(UINT* );
extern VOID m_cr_CpriMsgRcvProc2(UINT* );

extern VOID m_cr_CpriStaPriorityChk(USHORT linkno, USHORT , USHORT , USHORT , USHORT * );

extern VOID m_cr_HcTimeoutNtc( UINT*  );

extern VOID m_cr_HcTimeoutNtcRSP( UINT * buff_adr );

extern VOID m_cr_HcTimeoutNtcSND( UINT * buff_adr );

extern VOID m_cr_HcStateChgNtc(USHORT linkno, USHORT ,USHORT  );

extern VOID m_cr_cm_L3LnkStaNtc(USHORT linkno, USHORT ,USHORT );

extern VOID m_cr_cm_lnkDisNtc(USHORT system_type);

extern VOID m_cr_cm_lnkDisNtcRE(USHORT linkno, USHORT a_system_type);

extern VOID m_cr_DlinkStopReq(USHORT linkno);

extern VOID m_cr_HcReq( USHORT );

extern VOID m_cr_HcRsp(USHORT linkno,   USHORT   );

extern VOID m_cr_L3HcSta(USHORT linkno, USHORT a_system_type);

extern VOID m_cr_CpriConnectNtc(USHORT linkno, USHORT a_system_type);

extern VOID m_cr_CpriDisConnectNtc(USHORT linkno, USHORT a_system_type, USHORT state);

extern VOID m_cr_CpriStateChgNotice(USHORT linkno, USHORT cprista);

extern VOID m_cr_Cpri2ndRstReq(USHORT linkno);

extern VOID m_cr_L2DeactReq(USHORT linkno);

extern VOID m_cr_L2LinkStateProc(UINT * );
extern VOID m_cr_L2LinkStateProc2(UINT * );

extern VOID m_cr_L2LinkStateProcRE(UINT * );

extern VOID m_cr_CpriDscstTimer(USHORT linkno, UINT flg);

extern VOID m_cr_CpriDscstTimeoutNtc( UINT * buff_adr );

extern VOID  m_cr_ReComLnkSetProc(USHORT linkno, USHORT , USHORT );

extern VOID m_cr_cm_RunHisSet(UINT , UINT , UINT , UCHAR* );

extern VOID m_cr_cm_RunHisNG(USHORT linkno);

extern VOID m_cr_logPrint(FILE *outf);


extern VOID m_cr_CpriLinkPrepareRes();

extern VOID m_cr_CpriLinkPrepareReq(UINT* );



extern VOID m_cr_CpriStartReNotice(UINT* );

extern VOID m_cr_CpriStartRecNotice(UINT* );

extern VOID t_cr_CpriMonitorEndProc(UINT* );

extern VOID t_cr_CpriMonitorStrProc(UINT* );

extern VOID t_cr_CpriSimStartProc(UINT* );

extern VOID t_cr_CpriSimStopProc(UINT* );

extern VOID t_cr_forceStopProc(UINT* );

extern VOID m_cr_CpriStopReNotice(UINT *);

extern VOID m_cr_CpriRestartNtc(VOID);

#endif/*M_CR_FTYP*/
                                  
