/*!
 * @skip  $ld:$
 * @file  f_frw_flashSaveFwInfoReq.c
 * @brief Reception of Flash Erase request
 * @date  2015/08/14 TDIPS)sasaki create
 * @date  2015/08/18 TDIPS)sasaki update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frw_inc.h"


/*!
 *  @brief  Save FW Info
 *  @note   
 *  
 *  @retval 0:OK, 1:NG
 *  @date  2015/08/14 TDIPS)sasaki create
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

INT f_frw_flashSavefwInfo( VOID )
{
	INT			rtn;

	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frw_flashSavefwInfo] ENTER" );

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FW INFO ERASE Start");
	/********************************************************************************************/
	/* PS側QSPI-FLASH(0面:0x00F00000)のFW情報(REファイル×16)保存領域をイレース					*/
	/********************************************************************************************/
	rtn = BPF_HM_DEVC_QSPI_FLASH_ERASE( D_FRW_FWINFO_AREA_FACE, D_FRW_FWINFO_AREA_OFFSET, sizeof(f_frww_fwInfo));
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(
			f_frww_assert_p,
			D_RRH_LOG_AST_LV_ERROR,
			"BPF_HM_DEVC_QSPI_FLASH_ERASE(%x,%08x,%d) NG",
			D_FRW_FWINFO_AREA_FACE,
			D_FRW_FWINFO_AREA_OFFSET,
			sizeof(f_frww_fwInfo)
		);
		return D_SYS_NG;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FW INFO ERASE End & WRITE Start");

	/********************************************************************************************/
	/* PS側QSPI-FLASH(0面:0x00F00000)にFW情報(REファイル×16)を書込								*/
	/********************************************************************************************/
	rtn = BPF_HM_DEVC_QSPI_FLASH_WRITE(D_FRW_FWINFO_AREA_FACE, D_FRW_FWINFO_AREA_OFFSET, sizeof(f_frww_fwInfo), (UCHAR *)f_frww_fwInfo);
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(
			f_frww_assert_p,
			D_RRH_LOG_AST_LV_ERROR,
			"BPF_HM_DEVC_QSPI_FLASH_WRITE(%x,%08x,%d) NG",
			D_FRW_FWINFO_AREA_FACE,
			D_FRW_FWINFO_AREA_OFFSET,
			sizeof(f_frww_fwInfo)
		);
		return D_SYS_NG;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FW INFO WRITE End");
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frw_flashSavefwInfo] RETURN" );
	return D_SYS_OK;
}
/* @} */


/*!
 *  @brief  Process of FW Info Save request
 *  @note   
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2015/08/14 TDIPS)sasaki create
 *  @date  2015/08/18 TDIPS)sasaki MKレビュー指摘No.55対応
 *  @date  2015/08/18 TDIPS)sasaki MKレビュー指摘No.56対応
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frw_flashFwInfoSaveReq( VOID *pRcvMsg )
{
	T_SYS_FWINFO_SAVE_RSP	flashFwInfoSaveRsp;
	UINT					rtn = D_SYS_OK;
	USHORT					a_savepos;

	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_frw_flashFwInfoSaveReq] ENTER" );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FW INFO SAVE REQ Start");

	memset((CHAR *)(&flashFwInfoSaveRsp), 0, sizeof(flashFwInfoSaveRsp));
	a_savepos = ((T_SYS_FWINFO_SAVE_REQ*)pRcvMsg)->savepos - 1;

	/****************************************************************************/
	/* Edit RE File Info Table													*/
	/****************************************************************************/
	f_frww_fwInfo[a_savepos] = ((T_SYS_FWINFO_SAVE_REQ*)pRcvMsg)->fwinfo;

	/****************************************************************************/
	/* Save RE File Info Table into FLASH										*/
	/****************************************************************************/
	rtn = f_frw_flashSavefwInfo();
	if(rtn == D_SYS_OK)
	{
		/********************************************************************************************/
		/* FLASHに保存したREファイル情報を共有メモリテーブルへ展開								*/
		/********************************************************************************************/
		rtn = f_cmn_com_fw_info_re_set(a_savepos, &f_frww_fwInfo[a_savepos]);
		if( rtn != D_RRH_OK )
		{
			cmn_com_assert_th_get( f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_cmn_com_fw_info_re_set(%d) NG",  a_savepos);
		}
	}

	/****************************************************************************/
	/* Send FW Info Save Response to pf_fmng thread (result = ret) 				*/
	/****************************************************************************/
	flashFwInfoSaveRsp.thdIf.uiEventNo = D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP;
	flashFwInfoSaveRsp.thdIf.uiSrcTQueueID = ((T_SYS_FWINFO_SAVE_REQ*)pRcvMsg)->thdIf.uiDstTQueueID;
	flashFwInfoSaveRsp.thdIf.uiDstTQueueID = ((T_SYS_FWINFO_SAVE_REQ*)pRcvMsg)->thdIf.uiSrcTQueueID;
	flashFwInfoSaveRsp.thdIf.uiLength= sizeof(flashFwInfoSaveRsp);
	flashFwInfoSaveRsp.result = rtn;

	/* save running history */
	f_frw_cm_RunHisSet(D_FRW_RUNHIS_FLG_SND, (UCHAR *)&flashFwInfoSaveRsp);

	/* Edit and send Response to pf_fmng thread */
	f_com_msgQSendFHM( flashFwInfoSaveRsp.thdIf.uiDstTQueueID,
					(CHAR*)&flashFwInfoSaveRsp,
					sizeof(T_SYS_FWINFO_SAVE_RSP), 
					flashFwInfoSaveRsp.thdIf.uiSrcTQueueID,
					f_frww_thrState, 
					D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP );
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE FW INFO SAVE REQ End");
	cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_frw_flashFwInfoSaveReq] RETURN" );        
    return ;    
}
/* @} */

