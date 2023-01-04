/*!
 * @skip   $Id$
 * @file   m_dl_fhm_idle_sysp_dl_req.c
 * @brief  Event:REシステムパラメータ更新要求(L3/DLM←RE/REC), 対RE状態:アイドル.
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
 *  @brief  Event:REシステムパラメータ更新要求(L3/DLM←RE/REC), 対RE状態:アイドル.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2021/03/03 M&C) Update src based on No.27 - Return if 3G
 */

VOID m_dl_fhm_idle_sysp_dl_req(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwSysType;
	INT			ldwResult;

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_idle_sysp_dl_req] ENTER" );

	lwSysType = ((CMT_TSKIF_RESYSNEWREQ *)bufp)->dummy;

	if (CMD_SYS_3G == lwSysType)
	{
		return;
	}

	/* ファイル情報報告(システムパラメータ)通知再送カウンタ初期化(=0回) */
	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_tc = CMD_NUM0;
	/* ファイル情報報告通知(シスパラ)を対象配下REへ送信 */
	ldwResult = m_dl_fhm_sysp_fileinfo_req( cpri_no, lwSysType );

	if( CMD_OK == ldwResult )
	{
		/* タイマ起動 システムパラメータ情報報告確認タイマ */
		m_dl_fhm_sysp_fileinfo_timersta( cpri_no, lwSysType );
		/* Change 対RE状態 => ファイル情報報告応答(シスパラ)待ち */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILEINF_SYSPRM_WAIT;
	}

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

	return ;
}

/* @} */

