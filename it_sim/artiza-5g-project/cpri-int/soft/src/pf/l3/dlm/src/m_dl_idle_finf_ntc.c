/*!
 * @skip   $Id$
 * @file   m_dl_idle_finf_ntc.c
 * @brief  the processing for "ファイル情報報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/23 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイル情報報告通知" receiving at the アイドル REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01906
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/23 TDIPS)sasaki Update IT2問処N0.132対処
 *  @date   2015/10/26 TDIPS)sasaki M-RRU-ZSYS-01906 Update IT2問処No.139対処
 */
VOID m_dl_idle_finf_ntc(VOID* bufp, USHORT cpri_no)
{
	UINT	a_data		= CMD_NUM0;					/* 対象データ			*/
	UINT	a_sigchk;								/* result of checking signal */
	USHORT	a_sys_typ;
	USHORT	a_rsp_result[CMD_MAX_SYSNUM] = {CMD_INFRPT_NG, CMD_HDL_NG};
	CMT_TSKIF_CPRIRCV_FILEINFREQ *a_cpridat_p = NULL;/* CPRI信号データ部	*/

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_finf_ntc] ENTER" );

	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp;

	a_sigchk = m_cm_chksig(a_cpridat_p->cpridat_fileinfreq.signal_kind,&a_sys_typ);
	if(a_sigchk != CMD_RES_OK)
	{
		m_dl_cm_CRFileInfRspSnd(CMD_MAKER_RENG,a_sys_typ);

		BPF_COM_LOG_ASSERT_DWL(	D_RRH_LOG_AST_LV_ERROR, "validate signal NG(ret=%d, sys=%d)", a_sigchk, a_sys_typ);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* 対象データを取得 */
	a_data = a_cpridat_p->cpridat_fileinfreq.data;

	
	/**************************/
	/* パラメータチェック処理 */
	/**************************/
	/* 対象データがシスパラ、ファームウェア以外のとき */
	if((a_data != CMD_SYSPAR) && (a_data != CMD_FRM || a_sys_typ == CMD_SYS_3G))
	{
		/********************/
		/* CPRI信号送信処理 */
		/********************/
		/* 情報報告NGでファイル情報報告応答送信処理起動 */
		m_dl_cm_CRFileInfRspSnd(a_rsp_result[a_sys_typ], a_sys_typ);

		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "PARAMETER NG(date=%d)", a_data);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		
		return ;
	}

	/* 対象データがシステムパラメータのとき */
	if(a_data == CMD_SYSPAR)
	{
		m_dl_SysPro(bufp,a_sys_typ);
		m_dl_rl_finfrsp_waiting_sysp_proc(a_sys_typ);
	}
	/* the target is firmware */
	else
	{
		m_dl_FimPro(bufp,cpri_no);
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */

