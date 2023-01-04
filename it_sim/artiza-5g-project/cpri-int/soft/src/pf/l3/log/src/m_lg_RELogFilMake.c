/*!
 * @skip   $Id$
 * @file   m_lg_RELogFilMake.c
 * @brief  FHM障害ログファイルと取得済配下RE障害ログを書庫化処理.
 * @date   2015/08/06 FPT)Tunghv Create for FHM.
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
 *  @brief  配下RE障害ログのファイルにデータを追加処理.
 *  @note 
 *  @param  *a_filedatsnd [in]  the received data
 *  @param  a_size        [in]  the received data size
 *  @param  a_re_no       [in]  RE number 
 *  @param  a_rec_typ     [in]  the type of RE system 3G or LTE
 *  @return RETURN CODE
 *  @date    2015/08/27 FPT)Tunghv Create.
 *  @date    2015/11/17 FPT)Lay 障害ログ実装改善.
 */
/********************************************************************************************************************/

VOID m_lg_RELogFilMake( UCHAR *a_filedatsnd, UINT a_size, USHORT a_rec_typ, USHORT a_re_no )
{
                                            
    CHAR                          str[256];
    FILE                          *fp;
    USHORT                        lwNo;
    size_t                        fw_cnt;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, a_rec_typ, "[m_lg_RELogFilMake] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_RELogFilMake] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 配下RE番号の確認 */
    if ( a_re_no < D_RRH_CPRINO_RE_MIN || a_re_no > D_RRH_CPRINO_RE_MAX )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "RE Number invalid" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_RELogFilMake] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }
    lwNo = a_re_no - 1;

    /* 障害ファイル保存場所が/var/log/sendLog2Recの下である */
    memset( str, 0, sizeof(str) );
    snprintf( str, sizeof(str), "%s/%s", LGD_REFHM_TROUBLE_LOG_FOLDER_PATH, &gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwNo].filename[CMD_NUM0] );

#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s filenamepath: %s\n", __LINE__, __FUNCTION__, str);
#endif
    /* 出力されたファイルを開く */
    fp = fopen( str, "ab+" ); 
    if( fp == NULL )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "fopen error." );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s fopen error end\n", __LINE__, __FUNCTION__);
#endif
        return; 
    }

    //fseek( fp, 0, SEEK_END );

    /* 対象RE障害ログファイル書き込み追加をする    */
    fw_cnt = fwrite( a_filedatsnd, sizeof(CHAR), a_size, fp );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Wrote %d of %d size\n", __LINE__, __FUNCTION__, fw_cnt, a_size);
#endif
    if( fw_cnt < a_size )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "fwrite error." );
    }

    /* ファイルを閉じる  */
    fclose( fp );

    cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_RELogFilMake] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    
    return ;
}

/* @} */

