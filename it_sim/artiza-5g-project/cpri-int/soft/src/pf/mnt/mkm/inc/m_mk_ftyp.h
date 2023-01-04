/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_mk_ftyp.h
 *  @brief	Maker MT manage task common function prototype definition
 *  @date   2008/07/29 FFCS)zhengmh create
 *  @date   2013/11/25 FFCS)zhaodx modify for zynq
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/


#ifndef M_MK_FTYP
#define M_MK_FTYP


/* Main part */
extern VOID m_mk_Main(UINT source, UINT event, UINT *inf_p);

extern VOID m_mk_Init(UINT *inf_p);

extern VOID m_mk_tiltSetNtc(UINT eepvalue);

extern VOID m_mk_TcpServInitReq(UINT *inf_p);

extern VOID m_mk_TcpInit_Rsp(UINT *inf_p);

extern VOID m_mk_TServQuitNtc( UINT *inf_p);
	
extern VOID m_mk_TcpLink_Conn(UINT *inf_p);

extern VOID m_mk_TcpLink_Rel(UINT *inf_p);

extern VOID m_mk_TcpData_Rcv(UINT *inf_p);

extern VOID m_mk_TcpData_SndRsp(UINT *inf_p);

extern VOID m_mk_CpriMon_DataSnd(UINT *inf_p);

extern VOID m_mk_CpriMon_DataRcv(UINT *inf_p);

extern VOID m_mk_CpriSimu_DataSnd(UINT *inf_p);

extern VOID m_mk_L1linkstat_setSysType(UINT *inf_p);

extern VOID m_mk_L1linkstat_chgnfy(UINT *inf_p);
extern VOID m_mk_L1linkstat_chgnfyRE(UINT *inf_p);
extern VOID m_mk_cpristamsg_ntc(MNT_COM_TSKIF_CPRISTANTC_REC *cpristatTbl_p, USHORT link_num);

extern VOID m_mk_L2linkStat_ChgNfy(UINT *inf_p);
extern VOID m_mk_L2linkStat_ChgNfyRE(UINT *inf_p);

extern VOID m_mk_L3linkStat_ChgNfy(UINT *inf_p);
extern VOID m_mk_L3linkStat_ChgNfyRE(UINT *inf_p);

extern VOID m_mk_CardStat_ChgNfy(UINT *inf_p);

extern VOID m_mk_Abort(UINT *inf_p);

extern VOID m_mk_Nop(UINT *inf_p);

extern VOID m_mk_FileWtRsp(UINT *a_inf_p);

extern VOID m_mk_FileWaitTimTO(UINT *a_inf_p);

extern VOID m_mk_TRAFLWRTTO(UINT *a_inf_p);


/* Sub part */
extern UINT m_mk_Anlz(UINT srctsk, UINT event);

extern VOID m_mk_cm_SetSndMsg(UINT rxevent, VOID *inf_p, CMT_TSKIF_TCPSNDREQ **sndmsg_pp);

extern UINT mk_cm_ReadEeprom(USHORT slot, USHORT *data);

extern VOID m_mk_CpriMon_StrReq(UINT *inf_p, INT linkhandle);

extern VOID m_mk_CpriMon_StpReq(UINT *inf_p, INT linkhandle);

extern VOID m_mk_CpriSimu_StrReq(UINT *inf_p, INT linkhandle);

extern VOID m_mk_CpriSimu_StpReq(UINT *inf_p, INT linkhandle);

extern VOID m_mk_CpriSimu_DataRcv(UINT *inf_p, INT linkhandle);

extern VOID m_mk_REMTConn(UINT *inf_p, UINT link_flag, INT linkhandle);

extern VOID m_mk_REMTRelease(UINT *inf_p, UINT link_flag, INT linkhandle);

extern UINT	m_mk_REMTNgcodeTrans(UINT a_ret);

extern VOID m_mk_MonDatTO(UINT *buf_p);

extern VOID m_mk_cm_MonDatProc(UINT rxevent, VOID *inf_p, USHORT link_num);

extern VOID m_mk_FileDl_StrReq(UINT * a_inf_p );

extern VOID m_mk_FileDataRcv(UINT *a_inf_p);

extern VOID m_mk_DataEndNtcRcv(UINT *a_inf_p);

extern VOID m_mk_FileDlStrRsp(UINT func_id, UINT ng_code);

extern VOID m_mk_cm_DLEnd( VOID );

extern VOID m_mk_DataEndNtcRsp(UINT func_id, UINT ng_code);

extern VOID m_mk_HB(UINT *inf_p);


/* CPRI data receive part*/

extern VOID m_mk_subCardstatus();

extern VOID m_mk_unSubCardstatus();

extern VOID m_mk_subl2state(USHORT linkno);

extern VOID m_mk_unsubl2state(USHORT linkno);

#ifdef OPT_RRH_ZYNQ_REC
VOID m_mk_CpriMsg_DataRcv(UINT *inf_p);
#endif

VOID m_mk_IvtInf_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_SvInf_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_RstCtl_Req(UINT *inf_p, INT linkhandle);
VOID m_mk_FiberSts_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FiberSts_SetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_AisgSts_GetReq(UINT *inf_p);
VOID m_mk_AisgSts_SetReq(UINT *inf_p);

/* get/set API state for FHM-MT */
VOID m_mk_FHMState_Get(MKM_FHMMT_FHMSTATEGET_RES *pOutData);
VOID m_mk_REState_Get(MKM_FHMMT_RESTATEGET_RES *pOutData, int index);
VOID m_mk_FHMSts_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTFiberSts_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTFiberSts_SetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTRESts_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTUseSta_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTUseSta_SetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTRstCtl_Req(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTPortIn_GetReq(UINT *inf_p, INT linkhandle);
VOID m_mk_FHMMTPortIn_SetReq(UINT *inf_p, INT linkhandle);

extern VOID m_mk_SysParaChgNtc(UINT *inf_p);

#endif	/*M_MK_FTYP*/

