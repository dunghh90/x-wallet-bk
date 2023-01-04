/*!
 * @skip   $Id$
 * @file   m_dl_rl_refw_dl_ng.c
 * @brief  Event: REファイル更新要求(L3/DLM←RE/REC) RE status EC_DLM_MAIN_STN_FILEINF_REP_SYSPARA_CON 3
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
 *  @brief  Event: REファイル更新要求(L3/DLM←RE/REC) RE status EC_DLM_MAIN_STN_FILEINF_REP_SYSPARA_CON 3
 *  @note   -------7.5.1--------------
			--2
			"=>3 - keep cur sate
			(NG応答をRE/RECへ返す)"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 */

VOID m_dl_rl_refw_dl_ng(VOID* bufp, USHORT cpri_no)
{
	USHORT					lwSysType;
	CMT_TSKIF_REFILNEWRES 	lt_RE_file_update_respond;


	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_refw_dl_ng] ENTER" );

	lwSysType = ((CMT_TSKIF_REFILNEWREQ *)bufp)->sys_type;

	//new msg
	//REファイル更新応答(CMT_TSKIF_REFILNEWRES)
	//CMD_TSKIF_REFIRM_DL_RSP         0x4202
	memset(&lt_RE_file_update_respond, 0, sizeof(lt_RE_file_update_respond));

	lt_RE_file_update_respond.head.uiEventNo 	= CMD_TSKIF_REFIRM_DL_RSP;
	lt_RE_file_update_respond.result_code 		= CMD_NG;
	lt_RE_file_update_respond.dummy				= lwSysType;

	send_msg_to_RE(MSG_RE_FILE_UPDATE_RESPOND, &lt_RE_file_update_respond);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_refw_dl_ng] RETURN" );

}

/* @} */