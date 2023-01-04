/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReHCStop.c
 *  @brief  RE強制停止
 *  @date   
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"
/********************************************************************************************************************/
/**
 *  @brief    RE強制停止
 *  @param    linkno        [in ] CPRIリンク番号
 *  @param    system_type   [in ] 3G/LTE
 *  @param    stopFactor    [in ] 強制停止要因
 *  @return   none
 *  @date     2015/08/13 TDIPS) ikeda create
 *  @date     2015/09/25  TDIPS) uchida 仕様変更により送信先をRCTからREへ
 *  @date     2015/10/24 TDIPS)sasaki Warning対処
 *  @date     2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/********************************************************************************************************************/
VOID m_rc_ReHCStop(USHORT linkno, USHORT system_type, T_RRH_RESTOP_FACTOR stopFactor)
{
	UINT   					bufget_rslt;						/* バッファ獲得結果				*/
	UINT					a_rtn ;
	CMT_TSKIF_RECMPSTPNTC	*ntc;
	
	bufget_rslt = cm_BReq(CMD_BUFCA_TSKIF,  sizeof(CMT_TSKIF_RECMPSTPNTC), (VOID **)&ntc);
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(CMD_ASL_USELOW, bufget_rslt, "m_rc_ReHCStop cm_BReq NG");
		return;
	}

	ntc->head.uiEventNo     = CMD_TSKIF_RE_FORCESTOP_IND;
	ntc->head.uiDstPQueueID = D_RRH_PROCQUE_RE;
	ntc->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	ntc->head.uiDstTQueueID = 0;
	ntc->head.uiSrcTQueueID = 0;
	ntc->head.uiLength      = sizeof( CMT_TSKIF_RE_FORCESTOP_IND );
	ntc->link_num           = linkno;
	ntc->system_type        = system_type;
	ntc->stopFactor         = stopFactor;
	a_rtn = cm_Enter(ntc->head.uiDstPQueueID, CMD_QRB_NORMAL, CMD_TSKIF_RE_FORCESTOP_IND, ntc);
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
		return;
	}
	/* layer3状態を強制停止中に */
	f_cmn_com_layer3_set_each(linkno, system_type, E_RRH_LAYER3_RE_STOP);
	return ;
}
	
/** @} */
