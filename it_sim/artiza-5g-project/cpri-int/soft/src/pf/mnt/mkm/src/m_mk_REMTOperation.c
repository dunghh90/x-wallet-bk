/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_REMTOperation.c
 *  @brief  Operation for request from RE-MT.
 *  @date   2015/04/07 FFCS)Yuzhh create for TDD-SRE.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_Dbg.h"
#include "rrhApi_Svp.h"
#include "rrhApi_Cpri.h"
#include "f_sys_def.h"



static UINT mkm_FiberOutput = CMD_OFF;
/********************************************************************************************************************/
/**
 *  @brief   REMT: inventory information request
 *  @note    Get inventory information and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_IvtInf_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT						uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT						uiLength = CMD_NUM0; 	/* message body length 			*/
	MKT_REMTIVTINFREQ 			*pDataReq = NULL;
	MKT_TCPIF_REMTIVTINFRES 	*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF	*/
	INT							uiCnt = CMD_NUM0;
	USHORT						*pAddrReq = NULL;
	USHORT						*pAddrRes = NULL;
	USHORT						usRegAddr = CMD_NUM0;
	USHORT						usRegVal = CMD_NUM0;
	
	/* RE-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* RE-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "RE-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_REMTIVTINFREQ *)inf_p;
	pDataReq->RegNum = mnt_com_swap_UINT(pDataReq->RegNum);

	/* RE-MT read buffer response buffer request */
	uiLength = CMD_NUM5 * sizeof(UINT) + CMD_NUM2 * (pDataReq->RegNum) * sizeof(USHORT) ;
	uiRet = cm_BReq(CMD_BUFCA_MON, uiLength + sizeof(CMT_TSKIF_HEAD), (VOID **)&pMsgRes);
	/* Return value judge */
	if(uiRet != CMD_RES_OK)
	{
		/* Buffer request error,abort */
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"cm_BReq", "Buffer request error",
			uiRet, uiLength, CMD_NUM0);
	}


	pAddrReq = &(pDataReq->RegAddr);
	pAddrRes = &(pMsgRes->IvtInfRes.RegAddr);
	
	for(uiCnt = CMD_NUM0; uiCnt< pDataReq->RegNum; uiCnt++)
	{
		usRegAddr = mnt_com_swap_USHORT(*pAddrReq);
		
		if(usRegAddr < sizeof(cmw_tra_inv_tbl)/sizeof(USHORT))
		{
			usRegVal = ((USHORT*)&cmw_tra_inv_tbl)[usRegAddr];
		}
		else
		{
			usRegVal = CMD_NUM0;
		}
		
		*pAddrRes++ = *pAddrReq++;
		*pAddrRes++ = mnt_com_swap_USHORT(usRegVal);
	}

	pMsgRes->IvtInfRes.FunctionID = pDataReq->FunctionID;
	pMsgRes->IvtInfRes.SeqNo      = pDataReq->SeqNo;
	pMsgRes->IvtInfRes.Result     = mnt_com_swap_UINT(CMD_OK);
	pMsgRes->IvtInfRes.NGCode     = mnt_com_swap_UINT(CMD_OK);
	pMsgRes->IvtInfRes.RegNum     = mnt_com_swap_UINT(pDataReq->RegNum);
	
	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_REMT_IVTINFGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}	
}


/********************************************************************************************************************/
/**
 *  @brief   REMT: SV information request
 *  @note    Get SV0~9 and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_SvInf_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT						uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT						uiLength = CMD_NUM0; 	/* message body length 			*/
	MKT_REMTSVINFREQ 			*pDataReq = NULL;
	MKT_TCPIF_REMTSVINFRES 		*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;			/* Pointer of TCP data send req IF	*/
	USHORT                      cardsta1_mask_bit = D_SYS_ALLF_SHORT;
	USHORT                      cardsta3_mask_bit = D_SYS_ALLF_SHORT;
	USHORT                      cardsta4_mask_bit = D_SYS_ALLF_SHORT;
	USHORT                      cardsta0_mask_bit = D_SYS_ALLF_SHORT;
	USHORT                      cardsta2_mask_bit = D_SYS_ALLF_SHORT;

	/* RE-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* RE-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "RE-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_REMTSVINFREQ *)inf_p;

	/* RE-MT read buffer response buffer request */
	uiLength = sizeof(MKT_TCPIF_REMTSVINFRES);
	uiRet = cm_BReq(CMD_BUFCA_MON, uiLength, (VOID **)&pMsgRes);
	/* Return value judge */
	if(uiRet != CMD_RES_OK)
	{
		/* Buffer request error,abort */
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"cm_BReq", "Buffer request error",
			uiRet, uiLength, CMD_NUM0);
	}

	if( cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num == CMD_DEV_TYPE_NT2R ) 
	{
		/* for nt2r, pa 2 and pa 3 is default on */
		/* for nt2r, br 2 alm and br 3 alm is default off */
		/* for nt2r, sleep 2 and sleep 3 err is default off */
		cardsta1_mask_bit &= (~( 0x0800 | 0x8000 | 0x1000 | 0x0100 ));
		cardsta2_mask_bit &= (~( 0x0800 | 0x1000 ));
		cardsta3_mask_bit &= (~( D_RRH_PAOFF_BR2 | D_RRH_PAOFF_BR3 ));
		cardsta4_mask_bit = (~CMD_SLP2_3_ERR_BIT);
		if( (mkw_cardStat[4] & cardsta4_mask_bit) == 0)
		{
			/* if there is not err, mask err */
			cardsta0_mask_bit &= (~0x0001);
		}
		if( (mkw_cardStat[2] & cardsta2_mask_bit) == 0)
		{
			/* if there is not alm, mask alm */
			cardsta0_mask_bit &= (~0x0002);
		}

	}

	pMsgRes->SvInfRes.FunctionID = pDataReq->FunctionID;
	pMsgRes->SvInfRes.SeqNo      = pDataReq->SeqNo;
	pMsgRes->SvInfRes.Result     = mnt_com_swap_UINT(CMD_OK);
	pMsgRes->SvInfRes.NGCode     = mnt_com_swap_UINT(CMD_OK);
	pMsgRes->SvInfRes.SvInf[0]   = mnt_com_swap_UINT(mkw_cardStat[0] & cardsta0_mask_bit);
	pMsgRes->SvInfRes.SvInf[1]   = mnt_com_swap_UINT(mkw_cardStat[1] & cardsta1_mask_bit);
	pMsgRes->SvInfRes.SvInf[2]   = mnt_com_swap_UINT(mkw_cardStat[2] & cardsta2_mask_bit);
	pMsgRes->SvInfRes.SvInf[3]   = mnt_com_swap_UINT(mkw_cardStat[3] & cardsta3_mask_bit);
	pMsgRes->SvInfRes.SvInf[4]   = mnt_com_swap_UINT(mkw_cardStat[4] & cardsta4_mask_bit);

	m_mk_cm_SetSndMsg(MKD_REMT_SVINFGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	/* Send TCP data tx request to LAN tx task */
	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}	
}

/********************************************************************************************************************/
/**
 *  @brief   REMT: card reset control request
 *  @note    Card reset control and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_RstCtl_Req(UINT *inf_p, INT linkhandle)
{
	UINT						uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT						uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_REMTRSTREQ				*pDataReq = NULL;
	MKT_TCPIF_REMTRSTRES		*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	E_RRHAPI_RCODE				apiRet = E_API_RCD_OK;
	
	/* RE-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* RE-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "RE-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_REMTRSTREQ *)inf_p;

	/* RE-MT read buffer response buffer request */
	uiLength = sizeof(MKT_TCPIF_REMTRSTRES);
	uiRet = cm_BReq(CMD_BUFCA_MON, uiLength, (VOID **)&pMsgRes);
	/* Return value judge */
	if(uiRet != CMD_RES_OK)
	{
		/* Buffer request error,abort */
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"cm_BReq", "Buffer request error",
			uiRet, uiLength, CMD_NUM0);
		return;
	}

	BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "local reset");
	
	apiRet = rrhApi_Svp_Mnt_ResetCard(D_RRH_PROCQUE_PF, CMD_NUM0, D_RRH_NULL,
				D_API_LCL_RST, D_RRH_RSTFACT_RERST);	
	if(apiRet == E_API_RCD_OK)
	{
		pMsgRes->RstRes.Result = mnt_com_swap_UINT(MKD_RET_NORMAL);
	}
	else
	{
		pMsgRes->RstRes.Result = mnt_com_swap_UINT(MKD_RES_FAILED);
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "local reset error: %d", apiRet);
	}

	pMsgRes->RstRes.FunctionID = pDataReq->FunctionID;
	pMsgRes->RstRes.SeqNo      = pDataReq->SeqNo;
	pMsgRes->RstRes.NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	
	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_REMT_RST_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}	
}

/********************************************************************************************************************/
/**
 *  @brief   REMT: Fiber output state get request
 *  @note    Get Fiber output state and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_FiberSts_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT						uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT						uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_REMTFIBEROUTPUTGETREQ	*pDataReq = NULL;
	MKT_TCPIF_REMTFIBEROUTPUTGETRES		*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	
	/* RE-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* RE-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "RE-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_REMTFIBEROUTPUTGETREQ *)inf_p;

	/* RE-MT read buffer response buffer request */
	uiLength = sizeof(MKT_TCPIF_REMTFIBEROUTPUTGETRES);
	uiRet = cm_BReq(CMD_BUFCA_MON, uiLength, (VOID **)&pMsgRes);
	/* Return value judge */
	if(uiRet != CMD_RES_OK)
	{
		/* Buffer request error,abort */
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"cm_BReq", "Buffer request error",
			uiRet, uiLength, CMD_NUM0);
		return;
	}

	pMsgRes->FiberOutputGetRes.Result     = mnt_com_swap_UINT(MKD_RET_NORMAL);
	pMsgRes->FiberOutputGetRes.NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	pMsgRes->FiberOutputGetRes.FunctionID = pDataReq->FunctionID;
	pMsgRes->FiberOutputGetRes.SeqNo      = pDataReq->SeqNo;
	pMsgRes->FiberOutputGetRes.State      = mnt_com_swap_UINT(mkm_FiberOutput);
	
	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_REMT_FIBEROUTPUTGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}	
}

/********************************************************************************************************************/
/**
 *  @brief   REMT: Fiber output state set request
 *  @note    Set Fiber output state and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_FiberSts_SetReq(UINT *inf_p, INT linkhandle)
{
	UINT						uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT						uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_REMTFIBEROUTPUTSETREQ	*pDataReq = NULL;
	MKT_TCPIF_REMTFIBEROUTPUTSETRES		*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	E_RRHAPI_RCODE				apiRet = E_API_RCD_OK;
	
	/* RE-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* RE-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "RE-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_REMTFIBEROUTPUTSETREQ *)inf_p;

	/* RE-MT read buffer response buffer request */
	uiLength = sizeof(MKT_TCPIF_REMTFIBEROUTPUTSETRES);
	uiRet = cm_BReq(CMD_BUFCA_MON, uiLength, (VOID **)&pMsgRes);
	/* Return value judge */
	if(uiRet != CMD_RES_OK)
	{
		/* Buffer request error,abort */
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"cm_BReq", "Buffer request error",
			uiRet, uiLength, CMD_NUM0);
		return;
	}

	pDataReq->State = mnt_com_swap_UINT(pDataReq->State);

	/* Paramter Check */
	if((pDataReq->State != CMD_ON) &&(pDataReq->State != CMD_OFF))
	{
		pMsgRes->FiberOutputSetRes.Result = mnt_com_swap_UINT(MKD_RES_PARAMERR);
	}
	else
	{
		apiRet = rrhApi_Cpri_Mnt_FiberOutput(D_RRH_CPRINO_REC, D_RRH_PROCQUE_PF, 0, D_RRH_NULL, pDataReq->State);
		if(apiRet == E_API_RCD_OK)
		{
			pMsgRes->FiberOutputSetRes.Result = mnt_com_swap_UINT(MKD_RET_NORMAL);
			mkm_FiberOutput = pDataReq->State;
		}
		else
		{
			pMsgRes->FiberOutputSetRes.Result = mnt_com_swap_UINT(MKD_RES_FAILED);
		}
	}

	pMsgRes->FiberOutputSetRes.FunctionID = pDataReq->FunctionID;
	pMsgRes->FiberOutputSetRes.SeqNo      = pDataReq->SeqNo;
	pMsgRes->FiberOutputSetRes.NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	
	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_REMT_FIBEROUTPUTSET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}

/********************************************************************************************************************/
/**
 *  @brief   REMT: AISG connect information request
 *  @note    Get AISG connect information and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 */
/********************************************************************************************************************/
VOID m_mk_AisgSts_GetReq(UINT *inf_p)
{
	/* T.B.D */
}

/********************************************************************************************************************/
/**
 *  @brief   REMT: AISG connect information set request
 *  @note    Set AISG connect information and response it to REMT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2015/04/07  FFCS)Yuzhh Create
 */
/********************************************************************************************************************/
VOID m_mk_AisgSts_SetReq(UINT *inf_p)
{
	/* T.B.D */
}


/* @} */
