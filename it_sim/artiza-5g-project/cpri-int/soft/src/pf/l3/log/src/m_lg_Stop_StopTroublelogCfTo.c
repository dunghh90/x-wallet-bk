/*!
 * @skip   $Id$
 * @file   m_lg_Stop_StopTroublelogCfTo.c
 * @brief  RE障害ログ取得中止確認タイムアウト処理.
 * @date   2015/07/31 FPT)Tunghv Create.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  RE障害ログ取得中止確認タイムアウト処理.
 *  @note
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return VOID
 *  @retval -
 * @date   2015/07/31 FPT)Tunghv Create.
 * @date   2015/10/27 TDIPS)Takeuchi 全てNGの場合「51:処理NG(RE)」
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */

VOID m_lg_Stop_StopTroublelogCfTo( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{

    USHORT        lwrslt_is_ng[CMD_MAX_SYSNUM] = {CMD_TRUE, CMD_TRUE};
    USHORT        lwLoop;
	UINT			a_timstart_result = CMD_RES_OK;
	INT				errcd = CMD_NUM0;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Stop_StopTroublelogCfTo] enter") ;
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

    /* 全てのREをループする (16 RE)*/
	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		/* RE障害ログ取得中止応答結果がOKの場合 */
		if(gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].result == CMD_NML)
		{
			lwrslt_is_ng[a_rec_typ] = CMD_FALSE;
		}
	}

	/* 全配下REのRE障害ログ取得中止応答結果がNGの場合 */
	if(lwrslt_is_ng[a_rec_typ] == CMD_TRUE)
	{
		/* RE障害ログ取得中止要求の再送回数が最大値未満 */
		if( LGD_RETRY_STOP_REQ_NUM_MAX > gw_lgw_REGetLogStopReqCnt )
		{
			/* RE障害ログ取得中止要求の再送 */
			/* 全てのREをループする (16 RE)*/
			for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
			{
			 	/* RE障害ログ取得状態が「配下REへ障害ログ中止要求中」の配下REの場合 */
				if( gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_STOPRESTS_REQUESTING )
				{
					/* 配下REへ要求送信 */
					m_lg_StopTroublelogL3SendMsg(a_rec_typ, lwLoop + 1);

					/* RE障害ログ取得中止応答結果がNGとして初期化 */
					gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].result = CMD_SET_NG;
            	}
			}

			/* 「RE障害ログ取得中止確認タイマ」開始 */
			a_timstart_result = cm_TStat(	E_RRH_TIMID_LOG_GET_STOP_LTE,	/* TimerID */
											CMD_TIMVAL_LOGGETSTOP,			/* Timer値 */
											CMD_TIMMD_SINGLE,				/* 1 Short Timer */
											CMD_TIMEOUTNTC_LOGGETSTPCF,		/* TO時EventNo */
											D_RRH_THDQID_L3_LOG,			/* TO時送信元ThreadQID */
											&errcd ) ;

			if( a_timstart_result != BPF_RU_HRTM_COMPLETE ){
				cm_Assert(D_RRH_LOG_AST_LV_ERROR, a_timstart_result, "[m_lg_StopTroublelog] cm_TStat Error") ;
			}

			/* RE障害ログ取得中止要求の再送回数のカウントアップ */
			gw_lgw_REGetLogStopReqCnt++;

			return;
		}
		/* RE障害ログ取得中止要求の再送回数が最大値以上 */
		else
		{
	    	/* RE障害ログ取得中止応答 (51:処理NG(RE))        */
			m_lg_send_stopres( a_rec_typ, CMD_HDL_NG ) ;
		}
	}
	else
	{
        /* RE障害ログ取得中止応答 (0:正常)    */
        m_lg_send_stopres( a_rec_typ, CMD_NML ) ;
    }

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* RE障害ログ取得状態が未処理 */
        gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_GETRESTS_NOTPROC;
    }

	/* RE障害ログ取得段階状態 ← 通常運用状態 */
    lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;

	/* RE障害ログ取得中止要求の再送回数初期化 */
	gw_lgw_REGetLogStopReqCnt = 0;

#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    return ;
}

/* @} */

