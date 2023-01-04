/*!
 * @skip   $Id$
 * @file   M_lg_nop.c
 * @brief  the processing for exception event.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the function processing for exception event.
 *  @note   This function is processed as follows.
 *          1) write exception log
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2015/11/10 FTP)Lay debug為のログ追加.
 */

VOID m_lg_nop( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
	CMT_TSKIF_HEAD*		rcvMsgP;
#endif
	/* アサートログ書込み			*/
	cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0,"[m_lg_nop] enter ") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
	rcvMsgP = (CMT_TSKIF_HEAD*)a_rcv_adr;
    printf( "[%d]%s <get_log_sta>%d <event>%x\n", __LINE__, __FUNCTION__,lgw_almlogmng_tbl[a_rec_typ].get_log_sta,rcvMsgP->uiEventNo);
#endif
	
	/* 例外イベントのため無処理		*/
	
	/* 状態遷移処理:状態遷移なし	*/
	
	return ;
}

/* @} */

