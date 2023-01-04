/*!
 * @skip   $Id$
 * @file   m_dl_rec_idle_fwrep_req.c
 * @brief  the processing for "firmware information report request" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/20 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"										/* MDIF common head file			  */
#include "m_dl_header.h"										/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "firmware information report request" receiving at the アイドル state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
VOID m_dl_rec_idle_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	UINT			ldwResult;
	INT				ldwerrcd;
	USHORT			lwTimerId;
	T_RRH_LAYER3    ltLayer3Sta; 		/* Layer3状態[17(対REC,対RE×16)] */

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start cpri_no = %d\n",__LINE__,__FUNCTION__, cpri_no);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_idle_fwrep_req] ENTER" );
	
	/* 共用メモリLayer3の呼び出し 取得用 */
	f_cmn_com_layer3_get(CMD_SYS_S3G, &ltLayer3Sta );
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s ltLayer3Sta.layer3_re[ cpri_no -1] = %d\n",__LINE__,__FUNCTION__, ltLayer3Sta.layer3_re[ cpri_no -1]);
	#endif
	
	/* RE状態が②RE起動中状態または④運用中状態の場合 */
	if(( DLD_L3_STA_4 == ltLayer3Sta.layer3_re[ cpri_no -1]) 
		|| ( DLD_L3_STA_2 == ltLayer3Sta.layer3_re[ cpri_no -1]))
	{
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s\n",__LINE__,__FUNCTION__);
		#endif
	
		/* 対RE状態#n(※1)をファームウェアファイル報告応答待ちへ遷移 */
		/* RE状態チェック */
		/* 配下CPRI番号 */
		((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->cprircv_inf.link_num = cpri_no;
		
		/* (0x4001)RE Forwarding Notice(ファームウェアファイル報告要求) */
		ldwResult = l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,  bufp, sizeof( CMT_TSKIF_CPRISND_FIRMFILEREQ ));
		
		/* ファームウェアファイル報告応答結果を初期化 */
		gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;

		/* 対RE状態#n(※1)をファームウェアファイル報告応答待ちへ遷移 */
		gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_FWREP_WAIT;
		
		/* LTE system*/
		lwTimerId = CMD_TIMID_FIRMFILE_REP + cpri_no - CMD_NUM1;
		
		/* ファームウェアファイル報告確認タイマ#n(※1)開始 */
		ldwResult = cm_TStat(lwTimerId, CMD_TIMVAL_FIRMFILE_REP, CMD_TIMMD_SINGLE,
									CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, &ldwerrcd);
		
	 	/*Start timer NG*/
		if(ldwResult != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_USELOW, lwTimerId, "ファームウェアファイル報告確認タイマ Start NG" );
		}
	}
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	
	return ;
}

/* @} */

