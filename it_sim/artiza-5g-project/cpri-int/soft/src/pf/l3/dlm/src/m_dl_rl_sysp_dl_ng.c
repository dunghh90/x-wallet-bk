/*!
 * @skip   $Id$
 * @file   m_dl_rl_sysp_dl_ng.c
 * @brief  Event:REシステムパラメータ更新要求(L3/DLM←RE/REC), 対REC状態:アイドル以外.
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
 *  @brief  Event:REシステムパラメータ更新要求(L3/DLM←RE/REC), 対REC状態:アイドル以外.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.257対応
 */

VOID m_dl_rl_sysp_dl_ng(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_sysp_dl_ng] ENTER" );

	lwSysType = ((CMT_TSKIF_RESYSNEWREQ *)bufp)->dummy;

	/* NG応答をRE/RECへ返す */
	m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG, lwSysType);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_sysp_dl_ng] RETURN" );

	return ;
}

/*!
 *  @brief  Send REシステムパラメータ更新応答 to Re/rec
 *  @note   
 *  @param  awCpriNo	[in]  the CPRI no
 *  @param  awRst 		[in]  Response result
 *  @param  awSystype 	[in]  Systerm type (3G/LTE)
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.257対応
 */
VOID m_dl_rl_sysp_dl_rsp(USHORT awCpriNo, USHORT awRst, USHORT awSystype)
{
	CMT_TSKIF_RESYSNEWRES		ltSysUpdRes;
	UINT						ldwResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_sysp_dl_rsp] ENTER" );

	cm_MemClr(&ltSysUpdRes, sizeof(CMT_TSKIF_RESYSNEWRES));
	/* Make msg response */
	/* EventNo: RE System Parameter Update Response */
	ltSysUpdRes.head.uiEventNo = CMD_TSKIF_RESYSPARA_UPDATE_RSP;
	/* CPRI番号 */
	ltSysUpdRes.cpri_no = awCpriNo;
	/* 結果コード */
	ltSysUpdRes.result_code = awRst;
	/* Systerm type */
	ltSysUpdRes.systype = awSystype;

	/* Send REシステムパラメータ更新応答 to Re/rec */
	ldwResult = l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltSysUpdRes, sizeof(CMT_TSKIF_RESYSNEWRES));
	
	if(D_RRH_OK != ldwResult)
	{
		cm_Assert( CMD_ASL_USELOW, ldwResult,"[m_dl_rl_sysp_dl_rsp] Send Msg NG" );
	}

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_sysp_dl_rsp] RETURN" );

	return ;
}
/* @} */

