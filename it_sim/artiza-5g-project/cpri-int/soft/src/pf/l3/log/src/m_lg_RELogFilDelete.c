/*!
 * @skip   $Id$
 * @file   m_lg_RELogFilDelete.c
 * @brief  FHM障害ログファイルと取得済配下RE障害ログを書庫化処理.
 * @date   2015/08/06 FPT)Tunghv Create.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2011
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/********************************************************************************************************************/
/**
 *  @brief  取得済配下RE障害ログファイルを削除処理.
 *  @note 
 *  @param  a_re_no    [in]  RE number 
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return RETURN CODE
 *  @date    2015/08/27 FPT)Tunghv Create for FHM.
 *  @date    2015/11/17 FPT)Lay 障害ログ実装改善.
 */
/********************************************************************************************************************/

VOID m_lg_RELogFilDelete( USHORT a_rec_typ, USHORT a_re_no )
{
                                            
    CHAR                         a_shcmd[512];
    INT                          ret;
    USHORT                       lwNo;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, a_rec_typ, "[m_lg_FhmRELogFilDelete] enter") ;
    
    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_FhmRELogFilDelete] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 配下RE番号の確認 */
    if ( a_re_no < D_RRH_CPRINO_RE_MIN || a_re_no > D_RRH_CPRINO_RE_MAX )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "RE Number invalid" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_FhmRELogFilDelete] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }
    lwNo = a_re_no - 1;

    memset( a_shcmd, 0, sizeof(a_shcmd) );
    snprintf( a_shcmd, sizeof(a_shcmd), "rm -f %s/%s", LGD_REFHM_TROUBLE_LOG_FOLDER_PATH, &obslog_filename_tbl[lwNo][0]);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s systemcmd:%s\n", __LINE__, __FUNCTION__, a_shcmd);
#endif
    ret = system( a_shcmd );
    if( D_RRH_OK != ret )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "command failed." );
    }

    cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_FhmRELogFilDelete] return") ;
    
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    return ;
}

/* @} */

