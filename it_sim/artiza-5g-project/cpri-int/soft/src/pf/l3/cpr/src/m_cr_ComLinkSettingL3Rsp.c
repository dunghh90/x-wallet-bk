/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_ComLinkSettingL3Rsp.c
 *  @brief  RE common link set response sending function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル					*/
#include "m_cr_header.h"  /* CPRI管理 task header file  */

/* @{ */
/**
* @brief RE common link set response sending function
* @note edit CPRI message RE common link set response and send to LAPB task.\n
* @param linkno [in] CPRIリンク番号
* @param SystemType [in] the destination system
* @param RspResult [in] RE common link set response result
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
*
*/

 VOID m_cr_ComLinkSettingL3Rsp(USHORT linkno, USHORT SystemType, USHORT RspResult)
 {
 	UINT		a_ret;	/* buffer hunt return code */
	CMT_TSKIF_CPRISND_LINKSTSRES*	a_linkstsres_p;	/* response buffer pointer */
	UINT		returncode;		/* L3 message sending function return code */

	a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRISND_LINKSTSRES), CMD_NUM1, (void**)&a_linkstsres_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_cr_ComLinkSettingL3Rsp", 
			"buffer hunt fail", a_ret, sizeof(CMT_TSKIF_CPRISND_LINKSTSRES), CMD_NUM0);
	}

	a_linkstsres_p->head.uiSrcPQueueID = D_RRH_PROCID_Q_L3;
	a_linkstsres_p->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_linkstsres_p->head.uiLength = sizeof(CMT_TSKIF_CPRISND_LINKSTSRES);
	a_linkstsres_p->cpridat_linkstsres.signal_kind = CMD_CPRID_LINKSTSRES + SystemType;
	a_linkstsres_p->cpridat_linkstsres.result = RspResult;
	a_linkstsres_p->cprisnd_inf.link_num = linkno;
	
	/************************************************************************/
	/* CPRI管理走行履歴書込み													*/
	/************************************************************************/
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_LPB, CMD_TSKIF_CPRISNDNTC, (UCHAR*)a_linkstsres_p);

	/*Send response to LAPB task */
	returncode = m_cm_L3MsgSend_REC((void *)a_linkstsres_p);
	if (returncode != CMD_L3_OK)
	{
		/* ASSERT */
		cm_Assert(CMD_ASL_USELOW, returncode, 
				"m_cm_L3MsgSend_REC NG");
	}
	
	return;
 }
/* @} */

/* @} */
