/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_FHMMTOperation.c
 *  @brief  Operation for request from FHM-MT.
 *  @date   2015/09/21 FPT)TuanVD create
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
#include "f_rrh_reg_cnt.h"

/**
 * @brief CPRI LINK LED(ACT LED) setting value
 * @note  CPRI LINK LED ACT table
 * @date  2015/09/30 FPT)TuanVD create
 */
const   T_RSV_LEDINFO f_fhmmt_ledCpriActInfo[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_CPRILEDSTATE_ACT_OFF,     D_RRH_CNT_FHM_ACTLED_OFF        },
    {   D_RRH_CPRILEDSTATE_ACT_BLNK,    D_RRH_CNT_FHM_ACTLED_CYCL       },
    {   D_RRH_CPRILEDSTATE_ACT_ON,      D_RRH_CNT_FHM_ACTLED_ON         }
};

/**
 * @brief CPRI LINK LED(DWN LED) setting value
 * @note  CPRI LINK LED ACT table
 * @date  2015/09/30 FPT)TuanVD create
 */
const   T_RSV_LEDINFO f_fhmmt_ledCpriDwnInfo[3] = {
        /*  LED state,                  Set Value   */
    {   D_RRH_CPRILEDSTATE_DWN_OFF,     D_RRH_CNT_FHM_DWNLED_OFF        },
    {   D_RRH_CPRILEDSTATE_DWN_BLNK,    D_RRH_CNT_FHM_DWNLED_CYCL       },
    {   D_RRH_CPRILEDSTATE_DWN_ON,      D_RRH_CNT_FHM_DWNLED_ON         }
};

/********************************************************************************************************************/
/**
 *  @brief   FHMMT: FHM state get
 *  @note    Get FHM state to FHM state struct
 *  @param   pOutData  [out] Pointer of output data
 *  @return   None
 *  @retval    -
 *  @date     2015/09/21  FPT)TuanVD Create
 *  @date     2015/12/07  tdips)enoki ALM要因数/ERR要因数の上限を255に設定
 */
/********************************************************************************************************************/
VOID m_mk_FHMState_Get(MKM_FHMMT_FHMSTATEGET_RES *pOutData)
{
	T_RRH_FHM_STATE				ltFhmSta;				/* FHM state */
	INT							indexInfo;

	/* Get FHM state from share memory to struct */
	f_cmn_com_fhm_state_get( &ltFhmSta );

	/* Get FHM Act info */
	if((ltFhmSta.FhmBase & CMD_CARDSV_ACT_BIT) == MKD_CARDSV_ACT_BIT)
	{
		pOutData->ActState = CMD_ON;
	}
	else
	{
		pOutData->ActState = CMD_OFF;
	}

	/* Get FHM error info */
	if((ltFhmSta.FhmBase & CMD_CARDSV_ERR_BIT) == MKD_CARDSV_ERR_BIT)
	{
		pOutData->ErrState = CMD_ON;
	}
	else
	{
		pOutData->ErrState = CMD_OFF;
	}

	/* Get FHM alm info */
	if((ltFhmSta.FhmBase & CMD_CARDSV_ALM_BIT) == MKD_CARDSV_ALM_BIT)
	{
		pOutData->AlmState = CMD_ON;
	}
	else
	{
		pOutData->AlmState = CMD_OFF;
	}

	/* Get FHM Use info */
	if((ltFhmSta.FhmBase & CMD_CARDSV_USE_BIT) == MKD_CARDSV_USE_BIT)
	{
		pOutData->UseState = CMD_ON;
	}
	else
	{
		pOutData->UseState = CMD_OFF;
	}
	
	/* FHM Alm info state */
	pOutData->Alm_num = ( ltFhmSta.alm_num <= 255 ) ? ltFhmSta.alm_num : 255;
	cm_MemClr(pOutData->Alm_info, sizeof(pOutData->Alm_info));
	for(indexInfo = 0; indexInfo < pOutData->Alm_num; indexInfo++)
	{
		pOutData->Alm_info[indexInfo] = (UINT)ltFhmSta.alm_inf[indexInfo];
	}

	/* FHM Err info state */
	pOutData->Err_num = ( ltFhmSta.err_num <= 255 ) ? ltFhmSta.err_num : 255;
	cm_MemClr(pOutData->Err_info, sizeof(pOutData->Err_info));
	for(indexInfo = 0; indexInfo < pOutData->Err_num; indexInfo++)
	{
		pOutData->Err_info[indexInfo] = (UINT)ltFhmSta.err_inf[indexInfo];
	}
}

/********************************************************************************************************************/
/**
 *  @brief   FHMMT: CPRI link state get info
 *  @note    Get CPRI link state to CPRI link state struct
 *  @param   pOutData  [out] Pointer of output data
 *  @param   linkno    [in ] CPRI link number
 *  @return   None
 *  @retval    -
 *  @date     2015/09/21  FPT)TuanVD Create
 */
/********************************************************************************************************************/
VOID m_mk_CPRILLinkSta_Get(MKM_FHMMT_CPRILEDSTA_RES *pOutData, int linkno)
{
	USHORT  usLoop;             /* for loop */
	UINT    regdat;             /* register data */
	UINT    actLedVal;          /* register data (mask) */
	UINT    dwnLedVal;          /* register data (mask) */
	UINT    actLedSta;          /* CPRI LINK LED(ACT LED) state */ /* pgr0000 */
	UINT    dwnLedSta;          /* CPRI LINK LED(DWN LED) state */ /* pgr0000 */

	/* Get register data from LED control register */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_RRH_REG_CNT_CPSLED(linkno), &regdat);
	actLedVal = regdat & D_RRH_CNT_FHM_ACTLED_MASK;
	dwnLedVal = regdat & D_RRH_CNT_FHM_DWNLED_MASK;

	for (usLoop = 0; usLoop < (sizeof(f_fhmmt_ledCpriActInfo)/sizeof(f_fhmmt_ledCpriActInfo[0])); usLoop++) {
	    if (actLedVal == f_fhmmt_ledCpriActInfo[usLoop].setVal) {
	        actLedSta = f_fhmmt_ledCpriActInfo[usLoop].ledSta; /* pgr0000 */
	        break;
	    }
	}
	pOutData->ActState = actLedSta; /* pgr0000 */

	for (usLoop = 0; usLoop < (sizeof(f_fhmmt_ledCpriDwnInfo)/sizeof(f_fhmmt_ledCpriDwnInfo[0])); usLoop++) {
	    if (dwnLedVal == f_fhmmt_ledCpriDwnInfo[usLoop].setVal) {
	        dwnLedSta = f_fhmmt_ledCpriDwnInfo[usLoop].ledSta; /* pgr0000 */
	        break;
	    }
	}
	pOutData->ErrState =  dwnLedSta; /* pgr0000 */
}
/********************************************************************************************************************/
/**
 *  @brief   FHMMT: RE state get info
 *  @note    Get RE state to RE state struct
 *  @param   pOutData  [out] Pointer of output data
 *  @param   index     [in ] CPRI link index
 *  @return   None
 *  @Bug_No   M-RRU-ZSYS-01981
 *  @date     2015/09/21  FPT)TuanVD Create
 *  @date     2015/11/17  FPT)Yen M-RRU-ZSYS-01981 fix bug IT3 No84 大きいサイズの変数をstaticに
 */
/********************************************************************************************************************/
VOID m_mk_REState_Get(MKM_FHMMT_RESTATEGET_RES *pOutData, int index)
{
	T_RRH_REST_S3G				ltReSta;
	static T_RRH_CARDST_S3G		ltS3gCardStaLte;		/* RE state for S3G */
	T_RRH_CARDST_3G				lt3gCardState;			/* RE state for 3G */

	/* get re S3G card status from share memory */
	f_cmn_com_cardst_s3g_get(index+1, &ltS3gCardStaLte);
	
	/* get re 3g card status from share memory */
	f_cmn_com_cardst_3g_get(index + 1, &lt3gCardState);

	/* check for ACT bit info */
	if ((ltS3gCardStaLte.card_svitem & MKD_CARDSV_ACT_BIT) == MKD_CARDSV_ACT_BIT)
	{
		pOutData->ActState = CMD_ON;
	}
	else
	{
		pOutData->ActState = CMD_OFF;
	}
	/* check for USE bit info */
	if ((ltS3gCardStaLte.card_svitem & MKD_CARDSV_USE_BIT) == MKD_CARDSV_USE_BIT
		|| (lt3gCardState.card_svitem & MKD_CARDSV_USE_BIT) == MKD_CARDSV_USE_BIT)
	{
		pOutData->UseState = CMD_ON;
	}
	else
	{
		pOutData->UseState = CMD_OFF;
	}
	/* check for ALM bit info */
	if ((ltS3gCardStaLte.card_svitem & MKD_CARDSV_ALM_BIT) == MKD_CARDSV_ALM_BIT)
	{
		pOutData->AlmState = CMD_ON;
	}
	else
	{
		pOutData->AlmState = CMD_OFF;
	}
	/* check for ERR bit info */
	if ((ltS3gCardStaLte.card_svitem & MKD_CARDSV_ERR_BIT) == MKD_CARDSV_ERR_BIT)
	{
		pOutData->ErrState = CMD_ON;
	}
	else
	{
		pOutData->ErrState = CMD_OFF;
	}
	/* check for AMP bit info */
	if ((ltS3gCardStaLte.card_extsvitem & MKD_TRA_AMPDWN_S3G_BIT) == MKD_TRA_AMPDWN_S3G_BIT)
	{
		pOutData->AmpState = CMD_ON;
	}
	else
	{
		pOutData->AmpState = CMD_OFF;
	}
	/* check for WUP bit info */
	if ((ltS3gCardStaLte.card_extsvitem & MKD_TRA_WARMUP_BIT) == MKD_TRA_WARMUP_BIT)
	{
		pOutData->WupState = CMD_ON;
	}
	else
	{
		pOutData->WupState = CMD_OFF;
	}

	/* get re status from share memory */
	f_cmn_com_rest_s3g_get(index + 1, &ltReSta);
	/* check for FAN info */
	if (ltReSta.signal_kind)
	{
		if (ltReSta.fan_num)
		{
			pOutData->FanState = CMD_ON;
		}
		else
		{
			pOutData->FanState = CMD_OFF;
		}
	}
}
/********************************************************************************************************************/
/**
 *  @brief   FHMMT: FHM state get request
 *  @note    Get FW, HW, FHM state, RE state, CPRI link state and response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/09/21  FPT)TuanVD Create
 *  @date     2015/10/24 TDIPS)sasaki Warning対処
 */
/********************************************************************************************************************/
VOID m_mk_FHMSts_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT	uiRet = CMD_RES_OK;	/* Return value of buffer request 		*/
	UINT	uiLength = CMD_NUM0; 	/* message body length 				*/
	MKT_FHMMT_STATEGET_REQ		*pDataReq = NULL;
	MKT_FHMMT_STATEGET_RES		*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	INT				index;
	UCHAR				start_flag = 0;	
	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_FHMMT_STATEGET_REQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
	uiLength = sizeof(MKT_FHMMT_STATEGET_RES);
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
	/* Get device type */
	memset(&(pMsgRes->DeviceKind[CMD_NUM0]), 0 , D_RRH_CARD_NAME_LEN);
	USHORT tmp_cardname[D_RRH_CARD_NAME_LEN/2];
	for(index = 0; index < (D_RRH_CARD_NAME_LEN/2 ); index++)
	{
		tmp_cardname[index] = mnt_com_swap_USHORT(cmw_tra_inv_tbl.cmx_cardname[index]);
	}
	memcpy(&(pMsgRes->DeviceKind[CMD_NUM0]), (UCHAR*)tmp_cardname,D_RRH_CARD_NAME_LEN );

	/* Get hardware version */
	memset(&(pMsgRes->HWVer[CMD_NUM0]), 0 , sizeof(pMsgRes->HWVer));
	USHORT tmp_HWVer;
	tmp_HWVer = mnt_com_swap_USHORT(cmw_tra_inv_tbl.cmx_hver1);
	memcpy(&(pMsgRes->HWVer[CMD_NUM0]), (UCHAR*)&tmp_HWVer,1 );
	memcpy(&(pMsgRes->HWVer[CMD_NUM1]), ((UCHAR*)&tmp_HWVer)+1,1 );

	/* Get firmware version */
	start_flag = (UCHAR)cmw_tra_inv_tbl.cmx_com_bootmen;
	USHORT tmp_FWVer;
	memset(&(pMsgRes->FWVer[CMD_NUM0]), 0 , sizeof(pMsgRes->FWVer));
	if(start_flag == 0)
	{
		tmp_FWVer = mnt_com_swap_USHORT(cmw_tra_inv_tbl.cmx_f0sver[0]);
	}
	else
	{
		tmp_FWVer = mnt_com_swap_USHORT(cmw_tra_inv_tbl.cmx_f1sver[0]);
	}
	memcpy(&(pMsgRes->FWVer[CMD_NUM0]), (UCHAR*)&tmp_FWVer,1 );
	memcpy(&(pMsgRes->FWVer[CMD_NUM1]), ((UCHAR*)&tmp_FWVer)+1,1 );

	/* Set FHM state for message send */
	m_mk_FHMState_Get(&(pMsgRes->FHMState));

	/* Set RE state for message send */
	for(index = 0; index < D_RRH_CPRINO_RE_MAX; index++)
	{
		m_mk_REState_Get(&(pMsgRes->REState[index]), index);
	}

	/* Get CPRI Link state for message send */
	for(index = D_RRH_CPRINO_REC; index < D_RRH_CPRINO_NUM; index++)
	{
		m_mk_CPRILLinkSta_Get(&(pMsgRes->CPRIState[index]), index);
	}

	pMsgRes->Result     = mnt_com_swap_UINT(MKD_RET_NORMAL);
	pMsgRes->NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	pMsgRes->FunctionID = pDataReq->FunctionID;
	pMsgRes->SeqNo      = pDataReq->SeqNo;
	
	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_FHMMT_STATEGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/********************************************************************************************************************/
/**
 *  @brief   FHM: Fiber output state get request
 *  @note    Get Fiber output state and response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle 
 *  @return   None
 *  @retval    -
 *  @date     2015/09/21  FPT)TuanVD Create
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTFiberSts_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT	uiRet = CMD_RES_OK;	/* Return value of buffer request 		*/
	UINT	uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_FHMMT_FIBEROUTPUTGET_REQ	*pDataReq = NULL;
	MKT_FHMMT_FIBEROUTPUTGET_RES	*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	UINT				index;
	UINT				regVal;
	
	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_FHMMT_FIBEROUTPUTGET_REQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
	uiLength = sizeof(MKT_FHMMT_FIBEROUTPUTGET_RES);
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

	/* Get Fiber out put state of REC, RE# and put them to message send */
	for(index = 1; index < D_RRH_CPRINO_NUM; index++)
	{
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
		M_RRH_REG_CPRI_SFPCNT(index),&regVal);
		if((regVal & MKD_FIBERFORCE_BIT) == MKD_FIBERFORCE_BIT)
		{
			/*	強制出力ON状態	*/
			pMsgRes->FiberOutput[index - 1] = CMD_ON;
		}
		else
		{
			/*	強制出力OFF状態	*/
			pMsgRes->FiberOutput[index - 1] = CMD_OFF;
		}
	}

	pMsgRes->Result     = mnt_com_swap_UINT(MKD_RET_NORMAL);
	pMsgRes->NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	pMsgRes->FunctionID = pDataReq->FunctionID;
	pMsgRes->SeqNo      = pDataReq->SeqNo;

	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_FHMMT_FIBEROUTPUTGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/********************************************************************************************************************/
/**
 *  @brief   FHM: Fiber output state set request
 *  @note    Set Fiber output state and response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/09/21  FPT)TuanVD Create
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTFiberSts_SetReq(UINT *inf_p, INT linkhandle)
{
	UINT	uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT	uiLength = CMD_NUM0; 		/* message body length 				*/
	MKT_FHMMT_FIBEROUTPUTSET_REQ	*pDataReq = NULL;
	MKT_FHMMT_FIBEROUTPUTSET_RES	*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	E_RRHAPI_RCODE			apiRet = E_API_RCD_OK;
	INT				index;
	T_RRH_CONN_DISC				conn;
	
	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_FHMMT_FIBEROUTPUTSET_REQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
	uiLength = sizeof(MKT_FHMMT_FIBEROUTPUTSET_RES);
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
	index = pDataReq->FiberOutID + 1;
	
	pDataReq->State = mnt_com_swap_UINT(pDataReq->State);

	/* 配下REの使用/未使用を取得 */
	f_cmn_com_conn_disk_get(&conn);
	/* Paramter Check */
	if((pDataReq->State != CMD_ON) &&(pDataReq->State != CMD_OFF))
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI FiberSts set NG:linkno:%d/setval=%d", index, pDataReq->State);
		pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_PARAMERR);
	}
	else if((index<D_RRH_CPRINO_RE_MIN)||(index>D_RRH_CPRINO_RE_MAX))
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI FiberSts set NG:linkno:%d/setval=%d", index, pDataReq->State);
		pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_PARAMERR);
	}
	/* 配下REの未使用の場合 */
	else if( conn.conn_disc[pDataReq->FiberOutID] == CMD_OFF )
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI FiberSts set FAILED:linkno:%d/setval=%d", index, pDataReq->State);
		pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_FAILED);
	}
	else
	{
		apiRet = rrhApi_Cpri_Mnt_FiberOutput(index, D_RRH_PROCQUE_PF, 0, D_RRH_NULL, pDataReq->State);
		if(apiRet == E_API_RCD_OK)
		{
			BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI FiberSts set OK:linkno:%d/setval=%d", index, pDataReq->State);
			pMsgRes->Result = mnt_com_swap_UINT(MKD_RET_NORMAL);
		}
		else
		{
			BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI FiberSts set FAILED:ret=0x%08x/linkno:%d/setval=%d", apiRet, index, pDataReq->State);
			pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_FAILED);
		}
	}
	pMsgRes->FunctionID = pDataReq->FunctionID;
	pMsgRes->SeqNo      = pDataReq->SeqNo;
	pMsgRes->NGCode     = mnt_com_swap_UINT(CMD_NUM0);

	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_FHMMT_FIBEROUTPUTSET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/********************************************************************************************************************/
/**
 *  @brief   FHM: RE Alm, Err, Fan state get request
 *  @note    Get Alm, Err, Fan state of RE and response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/09/23  FPT)TuanVD Create
 *  @date     2015/11/17  FPT)Yen fix bug IT3 No84
 *  @date     2015/12/07  tdips)enoki ALM要因数/ERR要因数の上限を255に設定
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTRESts_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT							uiRet = CMD_RES_OK;		/* Return value of buffer request 	*/
	UINT							uiLength = CMD_NUM0; 	/* message body length 				*/
	MKT_FHMMT_REINFSTATEGET_REQ		*pDataReq = NULL;
	MKT_FHMMT_REINFSTATEGET_RES		*pMsgRes  = NULL;
	CMT_TSKIF_TCPSNDREQ 			*pMsgTcp  = NULL;		/* Pointer of TCP data send req IF	*/
	static T_RRH_CARDST_S3G 		ltCardSta;
	T_RRH_REST_S3G 					ltReSta;
	INT								index;
	INT								indexInfo;
	


	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_FHMMT_REINFSTATEGET_REQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
	uiLength = sizeof(MKT_FHMMT_REINFSTATEGET_RES);
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

	index = pDataReq->REIndex;
	/* get re card status from share memory */
	f_cmn_com_cardst_s3g_get(index + 1, &ltCardSta);
	if(ltCardSta.signal_kind)
	{
		/* put alm info to message */
		pMsgRes->Alm_num = ( ltCardSta.alm_num <= 255 ) ? ltCardSta.alm_num : 255;
		cm_MemClr(pMsgRes->Alm_inf, sizeof(pMsgRes->Alm_inf));
		for(indexInfo = 0; indexInfo < pMsgRes->Alm_num; indexInfo++)
		{
			pMsgRes->Alm_inf[indexInfo] = (UINT)ltCardSta.alm_inf[indexInfo];
		}

		/* put err info to message */
		pMsgRes->Err_num = ( ltCardSta.err_num <= 255 ) ? ltCardSta.err_num : 255;
		cm_MemClr(pMsgRes->Err_inf, sizeof(pMsgRes->Err_inf));
		for(indexInfo = 0; indexInfo < pMsgRes->Err_num; indexInfo++)
		{
			pMsgRes->Err_inf[indexInfo] = (UINT)ltCardSta.err_inf[indexInfo];
		}
	}

	/* get re status from share memory */
	f_cmn_com_rest_s3g_get(index + 1, &ltReSta);
	if(ltReSta.signal_kind)
	{
		pMsgRes->Fan_num = ltReSta.fan_num;
		for(index = 0; index < ltReSta.fan_num; index++)
		{
			pMsgRes->Fan_inf[index] = ltReSta.fan_inf[index];
		}
	}

	pMsgRes->FunctionID = pDataReq->FunctionID;
	pMsgRes->Result     = mnt_com_swap_UINT(MKD_RET_NORMAL);
	pMsgRes->SeqNo      = pDataReq->SeqNo;
	pMsgRes->NGCode     = mnt_com_swap_UINT(CMD_NUM0);

	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_FHMMT_RESTATEINFOGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/********************************************************************************************************************/
/**
 *  @brief   FHM: RE Use/Unuse state get request
 *  @note    Get Re Use/Unuse state and response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/09/25  FPT)TuanVD Create
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTUseSta_GetReq(UINT *inf_p, INT linkhandle)
{
	UINT	uiRet = CMD_RES_OK;	/* Return value of buffer request 		*/
	UINT	uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_FHMMT_REUSESTATEGET_REQ	*pDataReq = NULL;
	MKT_FHMMT_REUSESTATEGET_RES	*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	UINT				index;
	UCHAR				*eeprom_p;
	
	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_FHMMT_REUSESTATEGET_REQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
	uiLength = sizeof(MKT_FHMMT_REUSESTATEGET_RES);
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
	eeprom_p = (UCHAR*)(&cmw_tra_inv_tbl)+D_SYS_NUM_0x1A0;

	/* Get RE Use/unse state and put them to message send */
	for(index = 0; index < D_RRH_CPRINO_RE_MAX; index++, eeprom_p++)
	{
		pMsgRes->USEState[index] = *eeprom_p;
	}

	pMsgRes->Result     = mnt_com_swap_UINT(MKD_RET_NORMAL);
	pMsgRes->NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	pMsgRes->FunctionID = pDataReq->FunctionID;
	pMsgRes->SeqNo      = pDataReq->SeqNo;

	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_FHMMT_REUSEIFGET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/********************************************************************************************************************/
/**
 *  @brief   FHM: RE Use/Unuse state set request
 *  @note    Set Re Use/Unuse state response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/09/25  FPT)TuanVD Create
 *  @date     2015/10/24 TDIPS)sasaki Warning対処
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTUseSta_SetReq(UINT *inf_p, INT linkhandle)
{
	UINT	uiRet = CMD_RES_OK;	/* Return value of buffer request 		*/
	UINT	uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_FHMMT_REUSESTATESET_REQ	*pDataReq = NULL;
	MKT_FHMMT_REUSESTATESET_RES	*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	UINT				index;
	UCHAR				*eeprom_p;
	USHORT				mtIndx;
	USHORT				mLnkIndx;
	
	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_FHMMT_REUSESTATESET_REQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
	uiLength = sizeof(MKT_FHMMT_REUSESTATESET_RES);
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

	index = pDataReq->REIndex + 1;
	/* Check param */
	if((pDataReq->State != CMD_ON) &&(pDataReq->State != CMD_OFF))
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI USE/UNUSE set NG:linkno:%d/setval=%d", index, pDataReq->State);
		pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_PARAMERR);
	}
	else if((index<D_RRH_CPRINO_RE_MIN)||(index>D_RRH_CPRINO_RE_MAX))
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI USE/UNUSE set NG:linkno:%d/setval=%d", index, pDataReq->State);
		pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_PARAMERR);
	}
	else
	{
		/*	増設の場合	*/
		if(pDataReq->State == CMD_ON)
		{
			/* set sfp power ON */
			uiRet = rrhApi_Cpri_Mnt_SetSfpPwr(D_RRH_PROCQUE_PF, index);
			if(CMD_RES_OK == uiRet)
			{
					/*	PowerONが成功した場合は、500msecWaitする	*/
					usleep(500*1000);
			}
		}
		uiRet = rrhApi_Cpri_Mnt_SetReUse(D_RRH_PROCQUE_PF, index, pDataReq->State);
		if(CMD_RES_OK == uiRet)
		{
			pMsgRes->Result = mnt_com_swap_UINT(MKD_RET_NORMAL);
			eeprom_p = (UCHAR*)(&cmw_tra_inv_tbl)+(D_SYS_NUM_0x1A0+pDataReq->REIndex);
			*eeprom_p = pDataReq->State;
			mtIndx = pDataReq->REIndex / 6;
			mLnkIndx = pDataReq->REIndex % 6;
			mkw_cpristatM[mtIndx].stainfo[mLnkIndx].cpriconf = pDataReq->State;
			BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI USE/UNUSE set OK:linkno:%d/setval=%d", index, pDataReq->State);
		}
		else
		{
			BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "CPRI USE/UNUSE set FAILED:ret=0x%08x/linkno:%d/setval=%d", uiRet, index, pDataReq->State);
			pMsgRes->Result = mnt_com_swap_UINT(MKD_RES_FAILED);
		}
	}
	pMsgRes->NGCode     = mnt_com_swap_UINT(CMD_NUM0);
	pMsgRes->FunctionID = pDataReq->FunctionID;
	pMsgRes->SeqNo      = pDataReq->SeqNo;

	/* Send TCP data tx request to LAN tx task */
	m_mk_cm_SetSndMsg(MKD_FHMMT_REUSEIFSET_RES, pMsgRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if(uiRet != CMD_RES_OK)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}

/********************************************************************************************************************/
/**
 *  @brief   FHM-MT: card reset control request
 *  @note    Card reset control and response it to FHM-MT.
 *  @param   inf_p  [in] Pointer of buffer
 *  @param   linkhandle [in] TCP link handle
 *  @return   None
 *  @retval    -
 *  @date     2015/10/14  FPT)TuanVD Create
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTRstCtl_Req(UINT *inf_p, INT linkhandle)
{
	UINT						uiRet = CMD_RES_OK;		/* Return value of buffer request 		*/
	UINT						uiLength = CMD_NUM0; 	/* message body length 					*/
	MKT_REMTRSTREQ				*pDataReq = NULL;
	MKT_TCPIF_REMTRSTRES		*pMsgRes = NULL;
	CMT_TSKIF_TCPSNDREQ 		*pMsgTcp = NULL;		/* Pointer of TCP data send req IF		*/
	E_RRHAPI_RCODE				apiRet = E_API_RCD_OK;
	
	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not conncted");
		return;
	}
	
	pDataReq = (MKT_REMTRSTREQ *)inf_p;

	/* FHM-MT read buffer response buffer request */
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
 *  @brief		FHM: RE PORT状態取得要求
 *  @param		inf_p		[in]	Pointer of buffer
 *  @param		linkhandle	[in]	TCP link handle
 *  @return		None
 *  @date		2016/04/25 TDI)satou create
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTPortIn_GetReq(UINT *inf_p, INT linkhandle)
{
	MKT_FHMMT_REPORTSTATEGET_RES *pRes;		/* FHM-MTへの応答作成先 */
	CMT_TSKIF_TCPSNDREQ *pMsgTcp = NULL;	/* TCPデータ送信要求(FHM-MTへの応答用) */
	INT   bit_no;							/* Bit位置 */
	UINT  uiRet;							/* 戻り値評価用 */
	T_RRH_REPORTSTA_S3G shmRePort;

	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not connected");
		return;
	}

	uiRet = cm_BReq(CMD_BUFCA_MON, sizeof *pRes, (VOID**)&pRes);
	if (CMD_RES_OK != uiRet)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "cm_BReq", "Buffer request error", uiRet, sizeof *pRes, 0);
		return;
	}

	/********************************************/
	/* 共有メモリから情報取得 */
	/********************************************/
	f_cmn_com_RePortSta_s3g_allget(&shmRePort);
	for (bit_no = 0; bit_no < (sizeof(pRes->CpriNo)/sizeof(pRes->CpriNo[0])); bit_no++)
	{
		pRes->CpriNo[bit_no] = (UINT)shmRePort.cpri_no[bit_no];
	}

	for (bit_no = 0; bit_no < (sizeof(pRes->RE_Bit)/sizeof(pRes->RE_Bit[0])); bit_no++)
	{
		pRes->RE_Bit[bit_no] = (UINT)shmRePort.bit_no[bit_no];
	}

	/********************************************/
	/* 応答送信 */
	/********************************************/
	pRes->FunctionID = ((MKT_FHMMT_REPORTSTATEGET_REQ*)inf_p)->FunctionID;
	pRes->SeqNo      = ((MKT_FHMMT_REPORTSTATEGET_REQ*)inf_p)->SeqNo;
	pRes->Result     = mnt_com_swap_UINT(MKD_RET_NORMAL);
	pRes->NGCode     = 0;

	m_mk_cm_SetSndMsg(MKD_FHMMT_REPORTSTATEGET_RES, pRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if (CMD_RES_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/********************************************************************************************************************/
/**
 *  @brief		FHM: RE PORT状態設定要求
 *  @param		inf_p		[in]	Pointer of buffer
 *  @param		linkhandle	[in]	TCP link handle
 *  @return		None
 *  @date		2016/04/25 TDI)satou create
 */
/********************************************************************************************************************/
VOID m_mk_FHMMTPortIn_SetReq(UINT *inf_p, INT linkhandle)
{
	T_RRH_REPORTSTA_S3G shm_data;				/* 共有メモリに書き込む情報を作成する変数 */
	MKT_FHMMT_REPORTSTATESET_REQ *pReq;			/* FHM-MTからの要求 */
	MKT_FHMMT_REPORTSTATESET_RES *pRes;			/* FHM-MTへの応答作成先 */
	CMT_TSKIF_TCPSNDREQ *pMsgTcp = NULL;		/* TCPデータ送信要求(FHM-MTへの応答用) */
	UINT   eep_offset;							/* EEPROMオフセット */
	UCHAR  eep_data;							/* EEPROM書き込み値 */
	INT    idxBitNo;							/* Bit位置 */
	UINT   uiRet;								/* 戻り値評価用 */
	INT    iRet;								/* 戻り値評価用 */
	UINT   mntResult = MKD_RET_NORMAL;

	/* FHM-MT is disconnected,assert */
	if(mkw_REMTConnect == MKD_CONN_N)
	{
		/* FHM-MT not connected */
		cm_Assert(CMD_ASL_DBGLOW, mkw_REMTConnect, "FHM-MT Client is not connected");
		return;
	}

	pReq = (MKT_FHMMT_REPORTSTATESET_REQ*)inf_p;

	/********************************************/
	/* EEPROM書き込み */
	/********************************************/
	for (idxBitNo = 0; idxBitNo < (sizeof(pReq->CpriNo)/sizeof(pReq->CpriNo[0])); idxBitNo++)
	{
		eep_offset = M_RRH_EEP_REPORTIN_CPRI_NO(idxBitNo);
		eep_data   = (UCHAR)(pReq->CpriNo[idxBitNo]);
		iRet = BPF_HM_DEVC_EEPROM_WRITE(eep_offset, &eep_data);
		if (BPF_HM_DEVC_COMPLETE != iRet)
		{
			mntResult = MKD_RES_FAILED;
		}
	}

	for (idxBitNo = 0; idxBitNo < (sizeof(pReq->RE_Bit)/sizeof(pReq->RE_Bit[0])); idxBitNo++)
	{
		eep_offset = M_RRH_EEP_REPORTIN_RE_BIT(idxBitNo);
		eep_data   = (UCHAR)(pReq->RE_Bit[idxBitNo]);
		iRet = BPF_HM_DEVC_EEPROM_WRITE(eep_offset, &eep_data);
		if (BPF_HM_DEVC_COMPLETE != iRet)
		{
			mntResult = MKD_RES_FAILED;
		}
	}

	/********************************************/
	/* 共有メモリ書き込み */
	/********************************************/
	memset(&shm_data, 0, sizeof shm_data);
	for (idxBitNo = 0; idxBitNo < (sizeof(pReq->RE_Bit)/sizeof(pReq->RE_Bit[0])); idxBitNo++)
	{
		shm_data.cpri_no[idxBitNo] = pReq->CpriNo[idxBitNo];
		shm_data.bit_no [idxBitNo] = pReq->RE_Bit[idxBitNo];
	}
	f_cmn_com_portInputSetting_s3g(&shm_data);

	/********************************************/
	/* 応答送信 */
	/********************************************/
	uiRet = cm_BReq(CMD_BUFCA_MON, sizeof *pRes, (VOID**)&pRes);
	if (CMD_RES_OK != uiRet)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "cm_BReq", "Buffer request error", uiRet, sizeof *pRes, 0);
		return;
	}

	pRes->FunctionID = pReq->FunctionID;
	pRes->SeqNo      = pReq->SeqNo;
	pRes->Result     = mnt_com_swap_UINT(mntResult);
	pRes->NGCode     = 0;

	m_mk_cm_SetSndMsg(MKD_FHMMT_REPORTSTATESET_RES, pRes, &pMsgTcp);
	pMsgTcp->linkhandle = linkhandle;

	uiRet = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, pMsgTcp);
	if (CMD_RES_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGHIGH, "cm_Enter NG:%d", uiRet);
	}
}
/* @} */
