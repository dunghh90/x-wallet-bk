/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReStr_Req.c
 *  @brief  RE立ち上げ要求送信
 *  @date   2015/08/13 TDIPS)uchida create
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
 *  @brief    RE立ち上げ要求をREプロセスに送信する
 *  @param    linkno        [in ] CPRIリンク番号
 *  @param    system_type   [in ] 3G/LTE
 *  @return   none
 *  @date     TDIPS)uchida create
 *  @date     2015/08/27 TDIPS)satou レビュー指摘反映
 *  @date     2016/01/29 TDIPS)enoki 問題対応
 */
/********************************************************************************************************************/
VOID m_rc_ReStr_Req(USHORT linkno, USHORT system_type)
{
    UINT                bufget_rslt;        /* バッファ獲得結果     */
    UINT                a_rtn ;             /* 戻り値判定用         */
    CMT_TSKIF_RESTR_REQ *Restr_req;         /* RE立ち上げ要求作成先 */
	T_RRH_LAYER3 		layer3;				/* RE Layer3状態		*/
	UINT				AstCode;			/* AssertCode			*/

	/* 該当REのLayer3状態が																*/
	/* ①起動待ちアイドルの場合、Layer3状態を②起動中に更新し、RECスレッドに通知		*/
	/* ⑤運用中アイドルの場合、Layer3状態は更新せずにRECスレッドに通知					*/
	/* その他の場合は、破棄する															*/
	f_cmn_com_layer3_get( system_type, &layer3 );
	if(( E_RRH_LAYER3_RE_IDLE != layer3.layer3_re[linkno-1] )&&
	   ( E_RRH_LAYER3_RE_OPEIDLE != layer3.layer3_re[linkno-1] ))
	{
		/* ASSERT  Uper2Byte:LinkNo,  Lower2Byte:Laer3状態 */
		AstCode = linkno;
		AstCode <<= 16;
		AstCode |= layer3.layer3_re[linkno-1];
		cm_Assert(CMD_ASL_USELOW, AstCode, "RE SatrtUp Check NG" );
		return;
	}
	
	bufget_rslt = cm_BReq(CMD_BUFCA_TSKIF,  sizeof(CMT_TSKIF_RESTR_REQ), (VOID **)&Restr_req);
    if(bufget_rslt != CMD_RES_OK)
    {
        /* ASSERT */
        cm_Assert(CMD_ASL_USELOW, bufget_rslt, "m_rc_ReStr_Req cm_BReq NG");
        return;
    }
    Restr_req->head.uiEventNo     = CMD_TSKIF_RE_STARTUP_REQ;
    Restr_req->head.uiDstPQueueID = D_RRH_PROCQUE_RE;
    Restr_req->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
    Restr_req->head.uiDstTQueueID = 0;
    Restr_req->head.uiSrcTQueueID = 0;
    Restr_req->head.uiLength      = sizeof(CMT_TSKIF_RESTR_REQ);
    Restr_req->link_num           = linkno;
    Restr_req->system_type        = system_type;
    a_rtn = cm_Enter(Restr_req->head.uiDstPQueueID, CMD_QRB_NORMAL, CMD_TSKIF_RE_STARTUP_REQ, Restr_req);
    if(a_rtn != BPF_RU_IPCM_OK)
    {
        /* エラー時アサート */
        cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
        return;
    }

	if( E_RRH_LAYER3_RE_IDLE == layer3.layer3_re[linkno-1] )
	{
		/* 当該REのLayer3状態をRE起動中状態に更新 */
	    f_cmn_com_layer3_set_each(linkno, system_type, E_RRH_LAYER3_RE_START);
	}
	return;
}
/** @} */
