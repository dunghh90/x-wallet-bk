/*!
 * @skip   $Id$
 * @file   m_dl_rec_nop.c
 * @brief  
 * @date   2015/08/22 FPT)Yen Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */

/*!
 *  @brief 
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_rec_nop(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_nop] " );
	return ;
}

/*!
 *  @brief 
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2016/02/26 TDI)satou ログレベル変更. 未接続REのログが常に出力され、ログを埋め尽くしてしまうため
 */

VOID m_dl_rec_err(VOID* bufp, USHORT cpri_no)
{
	USHORT		 	lwStaNo;
	EC_DLM_MAIN_EVT lwEventId;
	USHORT 			lwSysType = CMD_SYS_3G;
	CHAR errdata[CMD_NUM48];
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_err] ENTER" );
	/* for 3G */
	/* 状態番号取得 */
    lwStaNo = m_dl_rec_ownstn_get(lwSysType, cpri_no);
	/* イベント取得 */
    lwEventId = m_dl_evt_get(lwSysType);
	/* 無効処理履歴 */
	memset( errdata, 0, sizeof(errdata));
	snprintf(errdata, CMD_NUM48, "[m_dl_rec_err][sys: %02d][cpr:%02d][stn:%02d][evt:%02d].",
											lwSysType,
														cpri_no,
																lwStaNo,
																			lwEventId );
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, errdata );
	
	/* for S3G */
	lwSysType = CMD_SYS_S3G;
	/* 状態番号取得 */
    lwStaNo = m_dl_rec_ownstn_get(lwSysType, cpri_no);
	/* イベント取得 */
    lwEventId = m_dl_evt_get(lwSysType);
	/* 無効処理履歴 */
	memset( errdata, 0, sizeof(errdata));
	snprintf(errdata, CMD_NUM48, "[m_dl_rec_err][sys: %02d][cpr:%02d][stn:%02d][evt:%02d].",
											lwSysType,
														cpri_no,
																lwStaNo,
																			lwEventId );
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, errdata );
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_err] RETURN" );
	return ;
}

/* @} */

