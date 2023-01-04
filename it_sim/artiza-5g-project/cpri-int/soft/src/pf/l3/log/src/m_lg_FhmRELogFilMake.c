/*!
 * @skip   $Id$
 * @file   m_lg_FhmRELogFilMake.c
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
 *  @brief  FHM障害ログファイルと取得済配下RE障害ログを書庫化処理.
 *  @note 
 *  @param  *a_rcv_adr [in]  -  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return RETURN CODE
 *  @Bug_No  M-RRU-ZSYS-01975
 *  @date    2015/08/06 FPT)Tunghv Create
 *  @date    2015/11/10 FPT)Yen M-RRU-ZSYS-01975 IT2問処169の対応 障害ログのフォルダ見直し
 *  @date    2015/11/13 FPT)Lay IT3問処75対応.
 *  @date    2015/11/17 FPT)Lay IT3問処78対応.
 *  @date    2015/11/17 FPT)Lay 障害ログ実装改善.
 */
/********************************************************************************************************************/
VOID m_lg_FhmRELogFilMake( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
                                            
    CHAR                         a_shcmd[512];
    CHAR                         lwfilepath[256];
    INT                          iFileSize = 0;
    INT                          ret;
    FILE                         *fp;
    UINT                         length;
    E_RRHAPI_RCODE      		 apiRet;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, a_rec_typ, "[m_lg_FhmRELogFilMake] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
    
    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_FhmRELogFilMake] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s a_rec_typ == CMD_SYS_3G end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* フォルダを書庫化する */
    memset( a_shcmd, 0x00, sizeof(a_shcmd) );
    snprintf( a_shcmd, sizeof(a_shcmd), "tar c -f /var/log/%s.tar %s/* > /dev/null 2>&1",
              lgw_almlogmng_tbl[a_rec_typ].logfile_name, LGD_REFHM_TROUBLE_LOG_FOLDER_PATH);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s systemcmd:%s \n", __LINE__, __FUNCTION__, a_shcmd);
#endif
    ret = system(a_shcmd);
    if( D_RRH_OK != ret )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, 0, "command failed." );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s systemcmd NG\n", __LINE__, __FUNCTION__);
#endif
    }

    /* 書庫化した後にRENAMEする */
    memset( a_shcmd, 0x00, sizeof(a_shcmd) );
    snprintf( a_shcmd, sizeof(a_shcmd), "mv -f /var/log/%s.tar /var/log/%s > /dev/null 2>&1",
              lgw_almlogmng_tbl[a_rec_typ].logfile_name, lgw_almlogmng_tbl[a_rec_typ].logfile_name );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s systemcmd:%s \n", __LINE__, __FUNCTION__, a_shcmd);
#endif
    ret = system(a_shcmd);
    if( D_RRH_OK != ret )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, 0, "command failed." );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s systemcmd NG\n", __LINE__, __FUNCTION__);
#endif
    }

    /* ファイルサイズを取得する	*/
    strncpy( a_shcmd, "/var/log/", sizeof(a_shcmd) );
    length = strlen( a_shcmd );
    strncat( a_shcmd, (const CHAR *)lgw_almlogmng_tbl[a_rec_typ].logfile_name, sizeof(a_shcmd) - length );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s filename:%s size:%d\n", __LINE__, __FUNCTION__, a_shcmd, length);
#endif
    fp = fopen( a_shcmd , "r");
    if( fp == NULL )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, 0, "command failed." );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s file open NG\n", __LINE__, __FUNCTION__);
#endif
    }
    else
    {
        fseek(fp, 0, SEEK_END);
        iFileSize = ftell( fp );
        fclose(fp);
    }

    if ( iFileSize < 0 )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, 0, "ftell erro" );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s file tell NG\n", __LINE__, __FUNCTION__);
#endif
    }
    else
    {
        /* 出力ファイルの実ファイルサイズを保存: FHMの運用ログファイルサイズを加える */
        lgw_almlogmng_tbl[a_rec_typ].realsize = iFileSize;
    }

    /* 出力したファイル名(Path付き)を保存 */
    strncpy( lwfilepath, "/var/log/", sizeof(lwfilepath) );
    length = strlen( lwfilepath );
    strncat( lwfilepath, (const CHAR *)lgw_almlogmng_tbl[a_rec_typ].logfile_name, sizeof(lwfilepath) - length );
    memcpy( lgw_almlogmng_tbl[a_rec_typ].logfile_name_path,
            lwfilepath,
            sizeof(lgw_almlogmng_tbl[a_rec_typ].logfile_name_path) );
    
    /* 障害ログファイル保存用フォルダのクリアを要求 */
    memset( lwfilepath, 0x00, sizeof(lwfilepath) );
    snprintf( lwfilepath, sizeof(lwfilepath), "%s/*", LGD_REFHM_TROUBLE_LOG_FOLDER_PATH);
    /* API:圧縮したFHMと配下REの障害ログファイル削除Call */
    apiRet = rrhApi_Log_Mnr_TroubleLogDelete( D_RRH_PROCQUE_L3,  /* 応答待ちQueueID  */
                       0,   /* 応答待ち時間 */
                       NULL,  /* 取得データポインタ  */
                       (UCHAR*)lwfilepath);  /* ファイル名 */
    if( apiRet != E_API_RCD_OK )
    {
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, apiRet, "rrhApi_Log_Mnr_TroubleLogDelete APIERROR(LTE)") ;
    }

    cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_FhmRELogFilMake] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Stored <filename>%s <realsize>%d end\n", __LINE__, __FUNCTION__, lgw_almlogmng_tbl[a_rec_typ].logfile_name_path, lgw_almlogmng_tbl[a_rec_typ].realsize);
#endif
    
    return ;
}

/* @} */

