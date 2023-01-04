/*!
 * @skip   $Id$
 * @file   m_lg_FhmLogFilMake.c
 * @brief  FHM障害ログファイル作成処理.
 * @date   2015/08/06 FPT)Tunghv.
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
 *  @brief  障害ログファイル作成処理
 *  @note
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return RETURN CODE
 *  @date    2015/08/06 FPT)Tunghv Create.
 */
/********************************************************************************************************************/

VOID m_lg_FhmLogFilMake( USHORT a_rec_typ )
{

    E_RRHAPI_RCODE                apiRet; /* API戻り値 */

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_FhmLogFilMake] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* API CALL(障害ログ作成要求)  */
    apiRet= rrhApi_Log_Mnr_TroubleLogGet( D_RRH_PROCQUE_L3, /* 応答待ちQueueID  */
                                          0,     /* 応答待ち時間     */
                                          NULL,  /* 取得データポインタ  */
                                          LGD_FHM_TROUBLE_LOG_NAME,  /* 作成ファイル名 */
                                          LGD_MAX_FHMRECOMPDAT);  /* MAX SIZE (17*1048576)  */

    /* API Callが正常に終わった場合にのみ状態遷移する    */
    if( apiRet == E_API_RCD_OK )
    {
        /* ファイル送信とファイル送信完了通知の送信先を更新    */
        lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_SEND].dstPQID = D_RRH_PROCQUE_L2;
        lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_SEND].dstTQID = 0;

        lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_FINISH].dstPQID = D_RRH_PROCQUE_L2;
        lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_FINISH].dstTQID = 0;
    }
    else
    {
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, apiRet, "[ALMLOG] m_lg_Idle_GetTroublelog APIERROR(LTE)") ;
    }

    cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_FhmLogFilMake] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}

/* @} */

