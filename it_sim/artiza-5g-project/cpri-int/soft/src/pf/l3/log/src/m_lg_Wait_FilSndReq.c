/*!
 * @skip   $Id$
 * @file   m_lg_Wait_FilSndReq.c
 * @brief  ファイルデータ送信要求処理.
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
 *  @brief  ファイルデータ送信要求処理
 *  @note 
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return Void
 *  @date    2015/08/06 FPT)Tunghv Create.
 */
/********************************************************************************************************************/

VOID m_lg_Wait_FilSndReq( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
                                            
    CMT_TSKIF_CPRIRCV_FILEDATSND  *a_parm_p = NULL ; /* 障害ログ取得要求型ポインタ */
    USHORT                        lwReReq;
    UINT                          size     = CMD_NUM0; /* データサイズ */
    //UINT                          sgmt_no  = CMD_NUM0; /* 分割番号 */
    UINT                          lwrcvsize_calc     = CMD_NUM0;  /* 計算機受信サイズ */
    UINT                          lwrcvsize          = CMD_NUM0;  /* 受信サイズ */

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Wait_FilSndReq] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* In case buffer is Null */
    if( NULL == a_rcv_adr )
    {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        /* end processing */
        return;
    }

    /* 受信メッセージをRE障害ログ取得要求型でかぶせる */
    a_parm_p = (CMT_TSKIF_CPRIRCV_FILEDATSND *)a_rcv_adr ;

    if( a_parm_p->cprircv_inf.link_num < D_RRH_CPRINO_RE_MIN ||
         a_parm_p->cprircv_inf.link_num > D_RRH_CPRINO_RE_MAX )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Cpri Link Number not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogGetRes] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }
    /* 該当配下RE設定 */
    lwReReq = a_parm_p->cprircv_inf.link_num - D_RRH_CPRINO_RE_MIN;

    /* 総受信ログサイズはトータルサイズ以上の場合 */
    if(gw_lgw_LogFilRcvSize[a_rec_typ][lwReReq] > LGD_MAX_COMPDAT)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "TroubleLog data size is over 1M" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_FilSndReq return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }

    /*「該当配下REのRE障害ログ取得状態が配下REからのファイルデータ送信中」 &&
     * 該当配下REに対して分割ファイルデータの保存を処理 */
    if ( gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReReq] == LGD_GETRESTS_FILSENDING ) 
    {
        /* 結果コード・スロット番号抽出 */
        //sgmt_no = (a_parm_p->cpridat_filedatsnd.sgmtno1<<CMD_NUM16) 
        //          +  a_parm_p->cpridat_filedatsnd.sgmtno2;      /* 分割番号 */
        size = (a_parm_p->cpridat_filedatsnd.datsize1<<CMD_NUM16) 
                  +  a_parm_p->cpridat_filedatsnd.datsize2; /* データサイズ */

        /* 受信サイズ算出                */
        /* 受信サイズ = トータルサイズ - 総受信ログサイズ            */
        lwrcvsize_calc = LGD_MAX_COMPDAT - gw_lgw_LogFilRcvSize[a_rec_typ][lwReReq];
        if(lwrcvsize_calc >= size )
        {
            lwrcvsize = size ;
        }
        else
        {
            cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "TroubleLog data size is over 1M" );
            cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_FilSndReq return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
            return ;
        }

        /* データをファイルに追加する　*/ 
        m_lg_RELogFilMake( &a_parm_p->cpridat_filedatsnd.fildat[0], lwrcvsize, a_rec_typ, lwReReq+1 );

        /* 対象配下REの取得ファイルデータサイズ更新 */
        gw_lgw_LogFilRcvSize[a_rec_typ][lwReReq] += lwrcvsize;
    }
    else
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "RE was not a target for process" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_FilSndReq return") ;
    }

#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    return ;
}

/* @} */

