/*!
 * @skip   $Id$
 * @file   m_lg_Wait_TroublelogSndCfTo.c
 * @brief  RE障害ログ送信確認タイマタイムアウト処理.
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
 *  @brief  RE障害ログ送信確認タイマタイムアウト処理
 *  @note 
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return Void
 *  @date    2015/07/22 FPT)Tunghv
 */
/********************************************************************************************************************/

VOID m_lg_Wait_TroublelogSndCfTo( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{

    USHORT        lwLoop;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Wait_TroublelogSndCfTo] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogSndCfTo] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* RE障害ログ取得状態が配下REからのファイルデータ送信中の各配下REに対しての処理 */
    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* RE障害ログ取得状態が配下REからのファイルデータ送信中の場合 */
        if(gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_FILSENDING)
        {
            gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_GETRESTS_NOTPROC;
        
            /* 該当配下REに対して保存中障害ログファイルを削除 */
            m_lg_RELogFilDelete( a_rec_typ, lwLoop+1 );
        }
    }
    
    /* FHM障害ログファイル作成 */
    m_lg_FhmLogFilMake(a_rec_typ);

    /* RE障害ログ取得段階状態 ← FHMログファイル作成中状態 */
    lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_ACHFILCREATING;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}

/* @} */

