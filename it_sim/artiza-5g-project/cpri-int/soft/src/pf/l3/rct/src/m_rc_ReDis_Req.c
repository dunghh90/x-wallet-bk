/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReDis_Req.c
 *  @brief  RE切断要求
 *  @date   2015/08/13 TDIPS)uchida create
 */
/********************************************************************************************************************/

/*****************************************************************************************************************//**
 *
 * @addtogroup RRH_L3_RCT
 * @{
 *
 ********************************************************************************************************************/

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief    RE切断要求をREプロセスに送信する
 *  @param    linkno        [in ] CPRIリンク番号
 *  @param    layer_kind    [in ] layer状態
 *  @param    system_type   [in ] 3G/LTE
 *  @return   None
 *  @date     2015/08/13 TDIPS)uchida create
 *  @date     2015/08/27 TDIPS)satou レビュー指摘反映
 *  @date     2015/09/25 TDIPS)uchida layer1 or 2の場合とlayer3での処理を追加
 *  @date     2015/08/20 FPT) Yen update (IT2 Bug No 99)
 *  @date     2015/11/09 TDIPS)sasaki IT2問処No.164対処
 */
/********************************************************************************************************************/

VOID m_rc_ReDis_Req(USHORT linkno, USHORT layer_kind, USHORT system_type)
{
    UINT                    bufget_rslt;    /* バッファ獲得結果 */
    UINT                    a_rtn;          /* 戻り値判定用     */
    CMT_TSKIF_REDIS_REQ     *Redis_req;     /* RE切断要求作成先 */
    T_RRH_LAYER3			layer3;			/* laryer3 状態  */

	bufget_rslt = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_RESTR_REQ), (VOID **)&Redis_req);
    if(bufget_rslt != CMD_RES_OK)
    {
        /* ASSERT */
        cm_Assert(CMD_ASL_USELOW, bufget_rslt, "m_rc_ReDis_Req cm_BReq NG");
        return;
    }

	f_cmn_com_layer3_get(system_type, &layer3);

	/* L3断、かつL3状態が運用中の場合 */
    if(layer_kind == E_RRH_RAYER_KIND_3 && layer3.layer3_re[linkno - 1] == E_RRH_LAYER3_RE_OPE)
    {
    	/* 運用中アイドル状態に変更 */
		f_cmn_com_layer3_set_each(linkno, system_type, E_RRH_LAYER3_RE_OPEIDLE);
    }
    /* それ以外(1 or 2の場合)　*/
    else
    {
		/* 起動待ちアイドル状態に変更 */
		f_cmn_com_layer3_set_each(linkno, system_type, E_RRH_LAYER3_RE_IDLE);
		/* RE起動完了フラグをクリア */
		re_startupRSP[linkno][system_type] = D_RRH_OFF;
	}
    Redis_req->head.uiEventNo     = CMD_TSKIF_RE_DISCONNECT_REQ;
    Redis_req->head.uiDstPQueueID = D_RRH_PROCQUE_RE;
    Redis_req->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
    Redis_req->head.uiDstTQueueID = 0;
    Redis_req->head.uiSrcTQueueID = 0;
    Redis_req->head.uiLength      = sizeof( CMT_TSKIF_REDIS_REQ );
    Redis_req->link_num           = linkno;
    Redis_req->layer_kind		  = layer_kind;
    Redis_req->system_type        = system_type;
    a_rtn = cm_Enter(Redis_req->head.uiDstPQueueID, CMD_QRB_NORMAL, CMD_TSKIF_RE_DISCONNECT_REQ, Redis_req);

    /* エラー時アサート */
    if(a_rtn != BPF_RU_IPCM_OK)
    {
        cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
        return;
    }
}

/*****************************************************************************************************************//**
 *
 * @} addtogroup RRH_L3_RCT
 *
 ********************************************************************************************************************/
