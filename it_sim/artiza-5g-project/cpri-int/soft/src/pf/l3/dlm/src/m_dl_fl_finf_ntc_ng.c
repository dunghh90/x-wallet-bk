/*!
 * @skip   $Id$
 * @file   m_dl_fl_finf_ntc_ng.c
 * @brief  the processing for "ファイル情報報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイル情報報告通知" receiving at the FHM自律DL処理中 REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/21 FPT)Yen update comment
 */

VOID m_dl_fl_finf_ntc_ng(VOID* bufp, USHORT cpri_no)
{
	USHORT			lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_finf_ntc_ng] ENTER" );
	
	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	if(lwSysType == CMD_SYS_3G)
	{
		/* (3G)(0x2220)ファイル情報報告応答(1:情報報告NG) */
		m_dl_cm_CRFileInfRspSnd(CMD_INFRPT_NG,lwSysType);
	}
	else
	{
		/* (LTE)(0x2221)ファイル情報報告応答(51:処理NG(RE)) */
		m_dl_cm_CRFileInfRspSnd(CMD_HDL_NG,lwSysType);
	}

	/* 2: FHM自律DL処理中 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_FHMDL;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_finf_ntc_ng] RETURN" );

	return ;
}

/* @} */

