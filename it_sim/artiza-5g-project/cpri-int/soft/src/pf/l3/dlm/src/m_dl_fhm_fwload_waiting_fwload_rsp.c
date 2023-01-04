/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwload_waiting_fwload_rsp.c
 * @brief  Event: REファイルデータ読込応答(L3/DLM←L3/FRW_RE) RE status 2 DLD_RESTA_RE_FILEREAD
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/10/19 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "f_sys_inc.h"

/*!
 *  @brief  Event: REファイルデータ読込応答(L3/DLM←L3/FRW_RE) RE status 2 DLD_RESTA_RE_FILEREAD
 *  @note   -------7.5.7----------------
			--1
			ref: m_dl_fl_load_refw_rsp.c
			--1.1

			OK応答(ファイル情報報告通知送信)=>4 DLD_RESTA_FILEINF_WAIT
			NG応答 =>0" DLD_RESTA_IDLE
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/11 FPT)Duong fix bug IT1
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */
VOID m_dl_fhm_fwload_waiting_fwload_rsp(VOID* bufp, USHORT cpri_no)
{

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fwload_waiting_fwload_rsp] ENTER" );

	//input
	USHORT lw_CPRI_no = cpri_no;
	T_API_FILE_LOAD_FW_DATA_RSP* lt_msg_p = (T_API_FILE_LOAD_FW_DATA_RSP*)bufp;

	//DLM
	USHORT lw_res = 0;
	/*読込NG
	 *read NG
	 */
	lw_res = lt_msg_p->data.result;

	if (lw_res == D_SYS_NG)
	{
		/*7-5-10 REファイル更新なし(パラメータ:NG)を全てのREファイルデータ読込中リンクに対して実行
		 *execute: not update RE file with all RE file data reading links
		 *do 7.5.10
		 */
		//NOTE: call m_dl_ResumeWaitRE()
		m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);

		return;
	}

	/*DL中REファイル読込状態を2:読込済に設定
	*set DL RE file read status to 2 DLRE_FILE_READ_COMPLETE
	*/
	set_DownLoading_RE_file_read_status(DLD_READ_DONE);

	/*	ファイル情報報告通知	*/
	m_dl_FileInfoReportNotice(lw_CPRI_no);

	return;
}
/** @} */
