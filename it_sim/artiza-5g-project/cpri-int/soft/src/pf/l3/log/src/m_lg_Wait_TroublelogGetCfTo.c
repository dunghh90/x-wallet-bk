/*!
 * @skip   $Id$
 * @file   m_lg_Wait_TroublelogGetCfTo.c
 * @brief  RE障害ログ取得確認タイマタイムアウト処理.
 * @date   2015/08/06 FPT)Tunghv.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup TRA_LOG
 * @{
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/********************************************************************************************************************/
/**
 *  @brief  RE障害ログ取得確認タイマタイムアウト処理
 *  @note
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return VOID
 *  @date    2015/08/06 FPT)Tunghv Create.
 *  @date    2016/03/18 TDI)satou 一部REが障害ログ取得応答T.Oした場合に、実際に送信するサイズとRECに通知するサイズが異なってしまう不具合を修正
 */
/********************************************************************************************************************/

VOID m_lg_Wait_TroublelogGetCfTo( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{

    USHORT        lwLoop;
    USHORT        lwHave_RETroublelogRequesting = CMD_FALSE;
    UINT          luiFilSizeTotal = CMD_NUM0;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Wait_TroublelogGetCfTo] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* FHMでは、3Gの障害ログ対応外にする */
    if( CMD_SYS_3G == a_rec_typ)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Other error, 3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogGetCfTo] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* RE障害ログ取得状態が取得要求中の配下REがある場合 */
        if (gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_REQUESTING)
        {
            /* タイマアウトですので、配下REのRE障害ログ取得状態が 取得要求中 →　未処理 */
            gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_GETRESTS_NOTPROC;

            lwHave_RETroublelogRequesting = CMD_TRUE;
        }
        /* トータルサイズ計算. ファイルデータ送信中か送信完了しているREの合計で算出する */
        if ((gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_COMP)
        ||  (gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_FILSENDING))
        {
            luiFilSizeTotal += gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].datsize;
        }
    }

    /* RE障害ログ取得状態が取得要求中の配下REがない場合 */
    if(lwHave_RETroublelogRequesting == CMD_FALSE)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "All of GetTroubleLog Response received" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogGetCfTo] return" );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* RE障害ログ送信確認タイマ」開始していない場合、
     * RE障害ログ取得応答がまだ来ない全ての配下REがある場合 */
    if( gw_lgw_RETroublelogSendTimerStartFlg != D_RRH_ON )
    {
        /* 障害ログファイル作成 (配下REの障害ログ取得応答が全てNGなので、
         *                       FHM自身の障害ログを作成するだけ) */
        m_lg_FhmLogFilMake(a_rec_typ);

        /* RE障害ログ取得段階状態 ←  ログファイル作成中状態 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_ACHFILCREATING;
    }

    /* RE障害ログ取得応答 (0:正常)  */
    m_lg_send_getres( CMD_OK, a_rec_typ, luiFilSizeTotal + LGD_MAX_COMPDAT );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}

/* @} */

