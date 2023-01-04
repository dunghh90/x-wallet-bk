/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fsnd_waiting_fsnd_itv_to.c
 * @brief  Event
 * @date   2015/08/22 FPT)Khiemnv1 Create

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
 *  @brief  ファイルデータ送信間隔タイマタイムアウト通知
 *  @note   -------7.5.8 timer--------------------------
			ファイルデータ送信間隔タイマタイムアウト通知
			--1
			ref: m_dl_fl_fsnd_itv_to.c
			--1.1
			"最後の分割データ送信=>7
			else=>6"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/11 FPT)Duong fix bug IT1
 */
VOID m_dl_fhm_fsnd_waiting_fsnd_itv_to(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fsnd_waiting_fsnd_itv_to] ENTER" );

	USHORT lw_last_part_data_sent = 0;

	lw_last_part_data_sent = m_dl_SendFileData(cpri_no);

	if(lw_last_part_data_sent)
	{
		m_dl_SendFileDataEnd(cpri_no);
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fsnd_waiting_fsnd_itv_to] RETURN" );
}
/* @} */
