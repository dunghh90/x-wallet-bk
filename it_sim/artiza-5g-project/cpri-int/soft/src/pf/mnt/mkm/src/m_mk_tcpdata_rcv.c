/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_tcpdata_rcv.c
 *  @brief   Analyze event code in received TCP data, then call different process func
 *  @date   2008/07/15  FFCS)zhengmh Create
 *  @date   2011/10/13  FFCS)Linlj  modify for 省電力対応
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "m_mk_header.h"			/*	Maker MT management task header 	*/

/********************************************************************************************************************/
/**
 *  @brief     Analyze event code in received TCP data, then call different process func.
 *  @note     Analyze event code in received TCP data, then call different process func according to different even code.
 *  @param  inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/15  FFCS)zhengmh Create
 *  @date     2011/10/13  FFCS)Linlj  modify for 省電力対応
 *  @date     2015/07/29  TDI)satou 「閉塞制御要求(TDD)」を削除
 *  @date     2015/08/08  TDI)satou 「送信出力設定要求(TDD)」削除
 *  @date     2015/08/08  TDI)satou 「送信出力保存要求(TDD)」削除
 *  @date     2015/08/08  TDI)satou 「キャリア情報取得要求(TDD)」削除
 *  @date     2015/08/12  TDI)satou 「PA ONOFF制御要求(TDD)」削除
 *  @date     2016/04/25  TDI)satou MKD_FHMMT_REPORTSTATEGET_REQ, MKD_FHMMT_REPORTSTATESET_REQを追加
 */
/********************************************************************************************************************/
VOID m_mk_TcpData_Rcv(UINT *inf_p)
{
	UINT 					evcode;		/* Event code		*/
	UINT 					*data_p;	/* Pointer of data	*/
	CMT_TSKIF_TCPDATNTC 	*datrcv_p;	/* Pointer of TCP data reception ntc IF*/

	datrcv_p = (CMT_TSKIF_TCPDATNTC *)inf_p;
	evcode = mnt_com_swap_UINT(datrcv_p->msgevent);
	data_p = (UINT *)(&(datrcv_p->linkhandle));

	switch(evcode)
	{
		case MKD_CPRISIMDATRCVREQ:		/* CPRI simulator data rx req	*/
		printf("\n[INFO][MT][%s:%d] func: %s, CPRI simulator data rx req  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_CpriSimu_DataRcv(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMT_IVTINFGET_REQ:	/* インベントリ情報取得要求(TDD)	*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_IVTINFGET_REQ /* インベントリ情報取得要求(TDD) \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_IvtInf_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMT_SVINFGET_REQ:		/* SV情報取得要求(TDD)			*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_SVINFGET_REQ  SV情報取得要求(TDD)	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_SvInf_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMT_FIBEROUTPUTGET_REQ:	/* 光出力状態取得要求(TDD)	*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_FIBEROUTPUTGET_REQ  光出力状態取得要求(TDD)	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FiberSts_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMT_FIBEROUTPUTSET_REQ:	/* 光出力状態設定要求(TDD)	*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_FIBEROUTPUTSET_REQ  光出力状態設定要求(TDD)	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FiberSts_SetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMT_AISGGET_REQ:		/* AISG状態取得要求(TDD)		*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_AISGGET_REQ  AISG状態取得要求(TDD)	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_AisgSts_GetReq(data_p);
			break;
		case MKD_REMT_AISGSET_REQ:		/* AISG状態設定要求(TDD)		*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_AISGSET_REQ  AISG状態設定要求(TDD)	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_AisgSts_SetReq(data_p);
			break;
		case MKD_REMTFLDLSTRREQ:			/* file download start request			*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMTFLDLSTRREQ  file download start request	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			if( mkw_REMTConnect != MKD_LOCALCON)
			{
				/* RE-MT not connected */
				cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "[m_mk_TcpData_Rcv]RE-MT Client is not local conncted");
				return;
			};
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FileDl_StrReq(data_p);
			break;
		case MKD_REMTTRAFLBLKSND:		/* TRA file block transmit 				*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMTTRAFLBLKSND  TRA file block transmit	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			if( mkw_REMTConnect != MKD_LOCALCON)
			{
				/* RE-MT not connected */
				cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "[m_mk_TcpData_Rcv]RE-MT Client is not local conncted");
				return;
			};
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FileDataRcv(data_p);
			break;
		case MKD_REMTFLDLCMPREQ:		/* TRA   File Download Complete  request			*/
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMTFLDLCMPREQ  TRA   File Download Complete  request	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			if( mkw_REMTConnect != MKD_LOCALCON)
			{
				/* RE-MT not connected */
				cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "[m_mk_TcpData_Rcv]RE-MT Client is not local conncted");
				return;
			};
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_DataEndNtcRcv(data_p);
			break;
		case MKD_CPRIMONSTRREQ:			/* CPRI monitor start req		*/
		printf("\n[INFO][MT][%s:%d] func: %s, CPRI monitor start req  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_CpriMon_StrReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_CPRIMONSTPREQ:			/* CPRI monitor stop req		*/
		printf("\n[INFO][MT][%s:%d] func: %s, CPRI monitor stop req  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_CpriMon_StpReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_CPRISIMSTRREQ:			/* CPRI simulator start req		*/
		printf("\n[INFO][MT][%s:%d] func: %s, CPRI simulator start req  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_CpriSimu_StrReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_CPRISIMSTPREQ:			/* CPRI simulator stop req		*/
		printf("\n[INFO][MT][%s:%d] func: %s, CPRI simulator stop req  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_CpriSimu_StpReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMTCONSTRREQ:			/* RE-MT Connection Startup Request		*/
			printf("\n[INFO][MT][%s:%d] func: %s, RE-MT Connection Startup Request  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_REMTConn(data_p, MKD_TCPDATA, datrcv_p->linkhandle);
			break;
		case MKD_REMTCONSTPREQ:			      /* RE-MT Connection Stop Response	*/
		printf("\n[INFO][MT][%s:%d] func: %s, RE-MT Connection Stop Response  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_REMTRelease(data_p, MKD_TCPDATA, datrcv_p->linkhandle);
			break;
		case MKD_HBREQ:
		printf("\n[INFO][MT][%s:%d] func: %s, Heartbeat Signal Request  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_HB((UINT *)datrcv_p);
			break;
		case MKD_FHMMT_FIBEROUTPUTSET_REQ:		/* FHM-MT Fiber output set request */
			data_p = (UINT *)datrcv_p->msgbody;
			printf("\n[INFO][MT][%s:%d] func: %s, FHM-MT Fiber output set request  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_FHMMTFiberSts_SetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_FIBEROUTPUTGET_REQ:		/* FHM-MT Fiber output get request */
			data_p = (UINT *)datrcv_p->msgbody;
			printf("\n[INFO][MT][%s:%d] func : %s, FHM-MT Fiber output get request  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_FHMMTFiberSts_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_STATEGET_REQ:			/* FHM-MT FHM state, RE state, CPRI link state get request */
			data_p = (UINT *)datrcv_p->msgbody;
			printf("\n[INFO][MT][%s:%d] func: %s, FHM-MT FHM state, RE state, CPRI link state get request  \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_FHMSts_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_REUSEIFGET_REQ:			/* FHM-MT RE use/unuse state get request */
			data_p = (UINT *)datrcv_p->msgbody;
			printf("\n[INFO][MT] [%s:%d] func: %s,FHM-MT RE use/unuse state get request   \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_FHMMTUseSta_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_REUSEIFSET_REQ:			/* FHM-MT RE use/unuse state set request */
			data_p = (UINT *)datrcv_p->msgbody;
			printf("\n[INFO][MT][%s:%d] func: %s, FHM-MT RE use/unuse state set request   \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_FHMMTUseSta_SetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_RESTATEINFOGET_REQ:		/* FHM-MT RE info get request */
			data_p = (UINT *)datrcv_p->msgbody;
			printf("\n[INFO][MT][%s:%d] func: %s, FHM-MT RE info get request   \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			m_mk_FHMMTRESts_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_REMT_RST_REQ:					/* FHM-MT reset request */
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_REMT_RST_REQ   FHM-MT reset request	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FHMMTRstCtl_Req(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_REPORTSTATEGET_REQ:
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_FHMMT_REPORTSTATEGET_REQ	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FHMMTPortIn_GetReq(data_p, datrcv_p->linkhandle);
			break;
		case MKD_FHMMT_REPORTSTATESET_REQ:
		printf("\n[INFO][MT][%s:%d] func: %s, MKD_FHMMT_REPORTSTATESET_REQ	 \n   ",__FILE__,__LINE__,__func__);//C9 add code debug
			data_p = (UINT *)datrcv_p->msgbody;
			m_mk_FHMMTPortIn_SetReq(data_p, datrcv_p->linkhandle);
			break;
		default:
		printf("\n[INFO][MT][%s:%d] func: %s, m_mk_TcpData_Rcv error:	 \n   ",__FILE__,__LINE__,__func__,__func__);//C9 add code debug
			cm_Assert(CMD_ASL_DBGLOW, evcode,
						"[m_mk_TcpData_Rcv]Event not found");
			return;
	}
}

/* @} */
