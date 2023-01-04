/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwload_waiting_fwloadstop_rsp.c
 * @brief  Event: 
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
 *  @brief  Event: REファイルデータ読込中止応答
 *  @note   -------7.5.18---------------------
			--1
			ref: m_dl_fl_load_refw_stop_rsp.c
			--1.1
			=>0
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 */

VOID m_dl_fhm_fwload_waiting_fwloadstop_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fwload_waiting_fwloadstop_rsp] ENTER" );

	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	/*REファイルデータ読込中状態のCPRI#検索*/
	if (DLD_RESTA_RE_FILEREAD == get_RE_status(lw_REFW_index))
	{
		/*更新中リンク情報の当該CPRI#ビットOFF
		*updating link info's CPRI bit OFF
		*/
		set_updating_link_info(lw_CPRI_no, 0);

		/*当該対RE-FW情報初期化
		*init RE FW info
		*/
		init_REFW(lw_REFW_index);
		set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);

		/*do (A)*/
		m_dl_ResumeWaitRE();
	}
}
/* @} */
