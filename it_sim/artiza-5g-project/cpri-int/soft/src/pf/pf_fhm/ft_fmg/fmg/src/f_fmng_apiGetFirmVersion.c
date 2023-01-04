/*!
 * @skip  $ld:$
 * @file  f_fmng_apiGetFirmVersion.c
 * @brief Reception of API get file version request
 * @date  2013/11/15 FFCS)linb create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"


/*!
 *  @brief  Reception of API Get Firm Version request
 *  @note   This function is processed as follows.
 *          - Get the current firm version
 *          - Send API Get Firm Version Response
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create
 *  @warning	N/A
 *  @FeatureID	PF_File-003-002-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_apiGetFirmVersion(void *pRcvMsg)
{
	/* Initialize */
	T_API_FILE_FIRM_VERSION_REQ	*ptThrdif = NULL;
	T_API_FILE_FIRM_VERSION_RSP fileVerRsp;	

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_apiGetFirmVersion] ENTER" );				
	
	memset(&fileVerRsp, 0, sizeof(fileVerRsp));
	
	/* Get Common thread header from Receivd message */
	ptThrdif = (T_API_FILE_FIRM_VERSION_REQ*)pRcvMsg;
	/* Save Response QId  */
	f_fmngw_verRespQid = ptThrdif->header.uiSrcPQueueID;

	/************************************************************/
	/* Edit and send Get Firm Version Response to l3_dlm thread		*/
	/************************************************************/
	fileVerRsp.header.uiEventNo		= D_API_MSGID_FILE_FIRM_VERSION_RSP;	/*!< Event  ID         */
	fileVerRsp.header.uiSignalkind	= 0;									/*!< Signal Kind       */
	fileVerRsp.header.uiDstPQueueID	= f_fmngw_verRespQid;					/*!< Destination P QID */
	fileVerRsp.header.uiDstTQueueID	= 0;									/*!< Destination T QID */
	fileVerRsp.header.uiSrcPQueueID	= D_RRH_PROCID_Q_PF;					/*!< Source P QID      */
	fileVerRsp.header.uiSrcTQueueID	= 0;									/*!< Source T QID      */
	fileVerRsp.header.uiLength		= sizeof(fileVerRsp);					/*!< Length            */
	fileVerRsp.data.usever			= f_fmngw_usever_tbl;
	fileVerRsp.data.stupver			= f_fmngw_stupver_tbl;
	fileVerRsp.data.year			= f_fmngw_useyear_tbl;
	fileVerRsp.data.mon_day			= f_fmngw_usemonthday_tbl;
	strcpy((CHAR*)fileVerRsp.data.stringVer, "SRE");								/*!< T.B.D			   */

	/* save running history */
	f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&fileVerRsp);

	/* Edit and send API File Update Response to pf_send thread */
	f_com_sendMsgToAplFHM( f_fmngw_verRespQid, &fileVerRsp,sizeof(fileVerRsp) ,
						D_API_MSGID_FILE_FIRM_VERSION_RSP, D_SYS_THDQID_PF_FRMG);

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_apiGetFirmVersion] RETURN" );				
	return ;
}
/* @} */

