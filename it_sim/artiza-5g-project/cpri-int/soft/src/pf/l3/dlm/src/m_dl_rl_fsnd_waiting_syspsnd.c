/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_syspsnd.c
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
 *  @brief  the processing for "ファイルデータ送信" receiving at the ファイルデータ送信(シスパラ)待ち REC主導
 *  ファームウェア報告応答集約中 state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)Yen Create
 *  @date   2015/09/21 FPT)Yen update comment
 *  @date   2021/03/03 M&C) Update src based on No.27 - Responde NG if 3G
 */

VOID m_dl_rl_fsnd_waiting_syspsnd(VOID* bufp, USHORT cpri_no)
{
	USHORT							lwSysType;
	CMT_TSKIF_CPRIRCV_FILEDATSND*	ltFileDataSnd_p;
//	USHORT							lwDivNo;
	ULONG							llDataSize;
	VOID*							lFildat_p	= NULL;		/* ファイルデータ			*/
	UCHAR*							dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] =
										{ dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g };

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_syspsnd] ENTER" );

	/* CPRI信号データ部取得 */
	ltFileDataSnd_p = (CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp;

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	if (CMD_SYS_3G == lwSysType)
	{
		m_dl_cm_CRFileInfRspSnd(CMD_OTHR_ERR_RE, lwSysType);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
//	/* 分割番号取得 */
//	lwDivNo = (ltFileDataSnd_p->cpridat_filedatsnd.sgmtno1 << DLD_BIT16) | 
//				ltFileDataSnd_p->cpridat_filedatsnd.sgmtno2;
//	
	/* データサイズ取得 */
	llDataSize = (ltFileDataSnd_p->cpridat_filedatsnd.datsize1 << DLD_BIT16) | 
				 ltFileDataSnd_p->cpridat_filedatsnd.datsize2;

	/* ファイルデータアドレス取得 */
	lFildat_p = ltFileDataSnd_p->cpridat_filedatsnd.fildat;

	/* ファイルデータ送信の内容をシスパラ領域(共有メモリ)に保存 */
//	cm_MemCpy(
//			(VOID*)(gt_dlw_mnt_all_reinf[lwSysType][CMD_NUM0].file_addr),
//			lFildat_p,
//			llDataSize);

	cm_MemCpy( (VOID*)(dlw_rcvsyspa_tbl[lwSysType]),
			   lFildat_p,
			   llDataSize);

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	/* 受信済み分割番号インクリメント */
	(*gt_dlw_rec_all_tbl[lwSysType]).rcv_div_no++;
	/* 受信済みデータサイズにファイルデータ送信のデータサイズを加算 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rcv_data_size += llDataSize;

	/* 4: ファイルデータ送信(シスパラ)待ち */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_SYSPARA_WAIT;	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_syspsnd] RETURN" );

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	return ;
}

/* @} */

