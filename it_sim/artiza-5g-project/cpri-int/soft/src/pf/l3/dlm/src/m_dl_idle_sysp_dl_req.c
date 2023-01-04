/*!
 * @skip   $Id$
 * @file   m_dl_idle_sysp_dl_req.c
 * @brief  Event:REシステムパラメータ更新要求(L3/DLM←RE/REC), 対REC状態:アイドル.
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
 *  @brief  Event:REシステムパラメータ更新要求(L3/DLM←RE/REC), 対REC状態:アイドル.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_idle_sysp_dl_req(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_idle_sysp_dl_req] ENTER" );

	lwSysType = ((CMT_TSKIF_RESYSNEWREQ *)bufp)->dummy;

	/* Change 対REC状態 => FHM自律DL処理中 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_FHMDL;
	/* 対RE状態遷移(FHM自律)へ入力する */
	m_dl_fhm_main(bufp, cpri_no);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_idle_sysp_dl_req] RETURN" );

	return ;
}

/* @} */

