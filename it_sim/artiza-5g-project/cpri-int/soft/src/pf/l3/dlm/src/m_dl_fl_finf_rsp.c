/*!
 * @skip   $Id$
 * @file   m_dl_fl_finf_rsp.c
 * @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対REC状態:FHM自律DL処理中.
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
 *  @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対REC状態:FHM自律DL処理中.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/14 FPT)DuongCD Update comment No.258
 */

VOID m_dl_fl_finf_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwReNo;
	USHORT		lwSysType;
	
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fl_finf_rsp] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	
	/* 対RE状態遷移(FHM自律)へ入力する */
	m_dl_fhm_main(bufp, cpri_no);

	for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
	{
		/* 対RE状態がアイドル以外 */
		if( DLD_RESTA_IDLE != gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].re_sta )
		{
			/* 対REC状態:FHM自律DL処理中 */
			/* 遷移なし */
			return;
		}
	}

	/* 対RE状態が全てアイドル */
	/* Change 対REC状態 => アイドル */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fl_finf_rsp] RETURN" );
	return ;
}


/* @} */

