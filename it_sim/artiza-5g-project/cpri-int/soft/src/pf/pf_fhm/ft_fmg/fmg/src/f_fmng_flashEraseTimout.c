/*!
 * @skip  $ld:$
 * @file  f_fmng_flashEraseTimout.c
 * @brief Reception of Flash erase timer out
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"

/*!
 *  @brief  Reception of Flash access timer out
 *  @note   This function is processed as follows.\n
 *          - Process time out.
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_flashAccessTimout(void *pRcvMsg)
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_flashAccessTimout] ENTER" );					

	if(f_fmngw_apikind == D_FMNG_API_ERA)
	{
		/* process for flash erasing time out */
		f_fmng_flashEraseTimout(pRcvMsg);
	}
	else if(f_fmngw_apikind == D_FMNG_API_WRT)
	{
		/* process for flash erasing time out */
		f_fmng_flashWriteTimout(pRcvMsg);
	}
	else
	{
		/* assert  */
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "ohter time out 0x%x,0x%x",
				f_fmngw_apikind,f_fmngw_thrdState);
	}
	/* reset the api kind */
	f_fmngw_apikind = D_FMNG_API_NONE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashAccessTimout] RETURN" );							
	return;
}



/*!
 *  @brief  Reception of Flash erase timer out
 *  @note   This function is processed as follows.\n
 *          - Send API File Start Response(Result = NG) to pf_send thread.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/06/18 ALP)murakami TDD-ZYNQ対応
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_flashEraseTimout(void *pRcvMsg)
{
	/* assert  */
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "flash erase T.O 0x%x,0x%x",f_fmngw_thrdState , f_fmngw_preThrdState);
		
	/* Send API Load Update Response (result = D_RRH_NG_TO)   */
	f_fmng_com_sndLoadEraseRsp(D_RRH_NG_TO,"flash erase T.O");

	/*************************************/
	/* if thread state is flash R/W waiting,              */
	/* also send NG response for the writing request */
	/*************************************/
	if(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWWAIT)
	{
		/* Send API Load Update Response (result =NG)   */
		f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR,"flash write abort");

		/* reset the saved writing request */
		memset((CHAR*)&f_fmngw_flashWaitWrtReq, 0 , sizeof(f_fmngw_flashWaitWrtReq));
		memset((CHAR*)&f_fmngw_flashWaitWrtReq_slave, 0 , sizeof(f_fmngw_flashWaitWrtReq_slave));	
	}

	/* Change thread state into running */
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

	return ;    
}
/* @} */

