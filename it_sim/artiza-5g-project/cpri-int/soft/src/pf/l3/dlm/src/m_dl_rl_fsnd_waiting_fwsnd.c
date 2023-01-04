/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_fwsnd.c
 * @brief  the processing for "ファイルデータ送信" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイルデータ送信" receiving at the ファイルデータ送信(FW)待ち REC主導
 *  ファームウェア報告応答集約中 state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *	@date   2015/08/20 FPT)VietCV update
 *	@date   2015/09/21 FPT)Yen update comment
 *	@date   2015/09/23 FPT)DuongCD IT update
 */

VOID m_dl_rl_fsnd_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{
	UINT	a_rtn		= CMD_RES_OK;			/* 戻り値格納変数			*/
	UINT	a_div_no	= CMD_NUM0;				/* 分割番号					*/
	UINT	a_datsize	= CMD_NUM0;				/* データサイズ				*/
	VOID*	a_fildat_p	= NULL;					/* ファイルデータ			*/
	CMT_TSKIF_CPRIRCV_FILEDATSND *a_cpridat_p = NULL;	/* CPRI信号データ部	*/
	USHORT							lwReNo;
	USHORT							lwLinkInfo;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_fwsnd] ENTER" );
	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp;
	/* 分割番号取得 */
	a_div_no = (a_cpridat_p->cpridat_filedatsnd.sgmtno1 << DLD_BIT16) | 
				a_cpridat_p->cpridat_filedatsnd.sgmtno2;
	
	/* データサイズ取得 */
	a_datsize = (a_cpridat_p->cpridat_filedatsnd.datsize1 << DLD_BIT16) | 
				 a_cpridat_p->cpridat_filedatsnd.datsize2;
	
	/* ファイルデータアドレス取得 */
	a_fildat_p = a_cpridat_p->cpridat_filedatsnd.fildat;
	
	/***********************************************/
	/* 条件チェック処理                            */
	/***********************************************/
	/* 実行中DL開始要因が0:未実行 */
	if( DLD_FACTOR_NONE == gt_dlw_dlm_tbl.dlstart_factor)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "dlstart_factor NG(fact=%d)", gt_dlw_dlm_tbl.dlstart_factor);
		return ;
	}
	
	if(a_div_no == CMD_NUM1)
	{
		/* 再ダウンロード準備 */
		m_dl_cm_DlRetry(CMD_SYS_S3G);
		
		/* get the handle again */
		if(dlw_handle == CMD_NUM0)
		{
			/* save the handle of shared memory */
			a_rtn = rrhApi_File_Fsm_loadMemory((INT *)&dlw_handle);
			if(a_rtn != E_API_RCD_OK)		
			{
				cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_loadMemory return NG");				return;
			}
		}
	}
	
	/* 受信済み分割番号をインクリメント[*4] */
	gt_dlw_rec_tbl.rcv_div_no++;
	
	/* 受信済みサイズにデータサイズ加算[*5] */
	gt_dlw_rec_tbl.rcv_data_size += a_datsize;
	
	/* ファイルデータをバッファリング */
	a_rtn = rrhApi_File_Fsm_SaveData(dlw_handle,a_fildat_p,a_datsize);
	if(a_rtn != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_SaveData NG");
	}
	
	if (DLD_FILENAME_RE == gw_dl_FileType)
	{
		for(lwReNo = CMD_NUM1; lwReNo <= CMD_NUM16; lwReNo++)
		{
			/* Get 更新中リンク情報 */
			lwLinkInfo = get_updating_link_info(lwReNo);
			/* 更新中リンク情報ONの場合 */
			if( CMD_ON == lwLinkInfo )
			{
				m_dl_rec_main(bufp, lwReNo);
			}
		}
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_fwsnd] RETURN" );
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s rcv_div_no[%d], rcv_data_size[%d] \n",__LINE__,__FUNCTION__, gt_dlw_rec_tbl.rcv_div_no, gt_dlw_rec_tbl.rcv_data_size);
	#endif
	return ;
}

/* @} */

