/*!
 * @skip  $ld:$
 * @file  f_fmng_init.c
 * @brief the entrance of pf_fmng task
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/11 TDIPS)sasaki update
 * @date  2015/08/20 TDIPS)sasaki update
 * @date  2015/08/21 TDIPS)sasaki update
 * @date  2015/09/16 TDIPS)sasaki update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"

/*!
 *  @brief  Task initialization process function
 *  @note   This function is processed as follows.\n
 *          - Initialize the internal table
 *          - Send SW Compatibility list Read Notification to pf_frsp
 *          - Change thread state
 *          - Send "Thread init completion to response" to pf_main
 *  @return VOID
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create
 *  @date  2015/08/11 TDIPS)sasaki update (Add initialization variable for RE-FLASH)
 *  @date   2015/06/16 ALP)murakami TDD-ZYNQ対応
 *  @date   2015/08/11 TDIPS)sasaki FHM対応
 *  @date   2015/08/20 TDIPS)sasaki REファイル用API種別保存変数の初期化を追加
 *  @date   2015/08/21 TDIPS)sasaki RE-FLASH WRITE/ERASE結果保存変数の初期化を追加
 *  @date   2015/09/16 TDIPS)sasaki 起動面情報、FHM保持FWファイル情報の共有メモリへの展開処理追加
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_fmng_init(VOID)
{
	T_SYS_INIT_COMP_NTC 	tInitCompNTC;
	T_RRH_FW_INFO			a_fwinfo;

	/* Local variable initialization  */
	memset((CHAR*)(&tInitCompNTC), 0,sizeof(tInitCompNTC));
	
	/**********************************************/
	/* RAM table initialization 				  */
	/**********************************************/
	f_fmngw_apikind = D_FMNG_API_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_dlwType = D_FMNG_DL_STARTUP;
	f_fmngw_preThrdState = D_SYS_NUM0;
	f_fmngw_re_preThrdState = D_SYS_NUM0;
	f_fmngw_read_cancel_re = D_SYS_OFF;	

	memset(&f_fmngw_runhis_tbl,0,sizeof(f_fmngw_runhis_tbl));
	
	f_fmngw_respQid	= 0;
	f_fmngw_verRespQid = 0;

	memset(f_fmngw_file_ver,0,sizeof(f_fmngw_file_ver));

	memset((CHAR*)&f_fmngw_flashWaitWrtReq, 0, sizeof(f_fmngw_flashWaitWrtReq));
	memset((CHAR*)&f_fmngw_flashWaitWrtReq_slave, 0, sizeof(f_fmngw_flashWaitWrtReq_slave));
	memset((CHAR*)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((CHAR*)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));

	/* get the firm bootface */
	f_fmngw_usebootface = f_cmw_tra_inv_tbl->cmx_com_bootmen & D_SYS_BIT1;
	f_fmngw_usebootface_cprifpga = f_cmw_tra_inv_tbl->cmx_cpri_bootmen & D_SYS_BIT1;

	/* Judge the programe startup side */
	if ((f_cmw_tra_inv_tbl->cmx_com_bootmen & D_SYS_BIT1) == D_SYS_NUM0)
	{
		/* get the current used version */
		f_fmngw_usever_tbl = f_cmw_tra_inv_tbl->cmx_f0sver[D_SYS_NUM0];
		/* get the next startup version */
		f_fmngw_stupver_tbl = f_fmngw_usever_tbl;
		/* get the current year */
		f_fmngw_useyear_tbl	= f_cmw_tra_inv_tbl->cmx_f0sver[D_SYS_NUM1];
		/* get the current month and day */
		f_fmngw_usemonthday_tbl = f_cmw_tra_inv_tbl->cmx_f0sver[D_SYS_NUM2];
	}
	else
	{
		/* get the current used version */
		f_fmngw_usever_tbl = f_cmw_tra_inv_tbl->cmx_f1sver[D_SYS_NUM0];
		/* get the next startup version */
		f_fmngw_stupver_tbl = f_fmngw_usever_tbl;
		/* get the current year */
		f_fmngw_useyear_tbl	= f_cmw_tra_inv_tbl->cmx_f1sver[D_SYS_NUM1];
		/* get the current month and day */
		f_fmngw_usemonthday_tbl = f_cmw_tra_inv_tbl->cmx_f1sver[D_SYS_NUM2];
	}

	/********************************************/
	/* 起動面情報の共有メモリへの展開処理追加	*/
	/********************************************/
	f_cmn_com_side_use_set((T_RRH_SIDE_USE *)&f_fmngw_usebootface);

	/********************************************/
	/* FHM保持FWファイル情報作成				*/
	/********************************************/
	a_fwinfo.maker_id = 'X';
	a_fwinfo.major_ver = ((f_fmngw_usever_tbl >> 12) * 10) + ((f_fmngw_usever_tbl >> 8) & 0xF);
	a_fwinfo.minor_ver = (((f_fmngw_usever_tbl >> 4) & 0xF) * 10) + (f_fmngw_usever_tbl & 0xF);
	a_fwinfo.year = ((f_fmngw_useyear_tbl >> 12) * 1000) + (((f_fmngw_useyear_tbl >> 8) & 0xF) * 100) + (((f_fmngw_useyear_tbl >> 4) & 0xF) * 10) + (f_fmngw_useyear_tbl & 0xF);
	a_fwinfo.month = ((f_fmngw_usemonthday_tbl >> 12) * 10) + ((f_fmngw_usemonthday_tbl >> 8) & 0xF);
	a_fwinfo.day = (((f_fmngw_usemonthday_tbl >> 4) & 0xF) * 10) + (f_fmngw_usemonthday_tbl & 0xF);

	/* FHM保持FWファイル情報(起動面)を共有メモリに展開	*/
	f_cmn_com_fw_info_fhm_set(f_fmngw_usebootface, &a_fwinfo);
	/* FHM保持FWファイル情報(待機面)を共有メモリに展開	*/
	f_cmn_com_fw_info_fhm_set((f_fmngw_usebootface ^ 0x1), &a_fwinfo);

	f_fmngw_assert_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"fmng thread");

	/************************************************************/
	/* Send "Thread init completion to response" to pf_main 	*/
	/************************************************************/
	/* 1. Edit the message header		*/
	tInitCompNTC.head.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
	tInitCompNTC.head.uiLength= sizeof(tInitCompNTC);

	/* 2. Send Task Init completion notice	*/
	f_fmngw_flashRsp[0] = D_SYS_OFF;
	f_fmngw_flashRsp[1] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_F_MAIN, 
					(CHAR*)&tInitCompNTC,
					sizeof( T_SYS_INIT_COMP_NTC ), 
					D_SYS_THDQID_PF_FRMG,
					f_fmngw_thrdState,
					D_SYS_MSGID_INIT_COMP_NTC );

	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_rsp_kind = D_FMNG_RSP_NONE;
	f_fmngw_re_fileUpdResult = D_SYS_OK;
	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;

	return ;	
}
/* @} */

