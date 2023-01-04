/*!
 * @skip  $ld:$
 * @file  f_fmng_anlz.c
 * @brief Analyze and dispatch message.
 *  @date 2013/11/14 FFCS)linb create
 *  @date 2015/08/14 TDIPS)sasaki update
 *  @date 2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.65対応
 *  @date 2015/08/20 TDIPS)sasaki MKレビュー指摘事項No.61対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

/*!
 *  @brief  Analyze and dispatch message
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2013/11/14 FFCS)linb create
 *  @date 2015/08/14 TDIPS)sasaki update
 *  @date 2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.65対応
 *  @date 2015/08/20 TDIPS)sasaki MKレビュー指摘事項No.61対応
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/

VOID f_fmng_anlz( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT 				uiIntEventNo;
	T_SYS_COMMON_THDIF 	*ptTskif;
	
	/************************************************************/
	/* Get event number from task IF header of message			*/
	/************************************************************/
	ptTskif = (T_SYS_COMMON_THDIF*)pRcvMsg;
	f_fmngw_extEventNo = ptTskif->uiEventNo;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	switch(f_fmngw_extEventNo)
	{
		/* API Firm Erase Request */
		case D_API_MSGID_FILE_FIRM_ERASE_REQ:
			uiIntEventNo = D_FMNG_EVTIDX_FIRMERASE_REQ;
			break;

		/* Flash Erase Response		*/
		case D_SYS_MSGID_FLASH_ERASE_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_FLSHERASE_RSP;
			break;

//		/* RE-FLASH Erase Response		*/
//		case D_SYS_MSGID_FLASH_RE_ERASE_RSP:
//			uiIntEventNo = D_FMNG_EVTIDX_FWINFO_DEL_START;
//			break;

		/* API Firm Updata Request	*/
		case D_API_MSGID_FILE_FIRM_UPDATE_REQ:
			if(((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data.fw_type != D_API_FILE_FW_TYPE_FHM)
			{
				return;
			}
			uiIntEventNo = D_FMNG_EVTIDX_FIRMUPD_REQ;
			break;

		/* Flash Write Response		*/
		case D_SYS_MSGID_FLASH_WRITE_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_FLSHWRT_RSP;
			break;

//		/* RE-FLASH Write Response		*/
//		case D_SYS_MSGID_FLASH_RE_WRITE_RSP:
//			uiIntEventNo = D_FMNG_EVTIDX_FWINFO_UPD_START;
//			break;

		/* API Get Firm Version 		*/
		case D_API_MSGID_FILE_FIRM_VERSION_REQ:
			uiIntEventNo = D_FMNG_EVTIDX_FIRMVER_REQ;
			break;

		/* API Cancel Update Firm	*/
		case D_API_MSGID_FILE_FIRM_CANCEL_IND:
			uiIntEventNo = D_FMNG_EVTIDX_FIRMCANCEL_IND;
			break;

		/* Timeout notification */
		case D_SYS_MSGID_FLASH_TIMEOUT_NTC:
			uiIntEventNo = D_FMNG_EVTIDX_FLSHACCESS_TOUT;
			break;

		/* Tar Process Response */
		case D_SYS_MSGID_TARPROCESS_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_TARPROCESS_RSP;
			break;

		/* FW Info Save Response		*/
		case D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_FWINFO_SAVE_RSP;
			break;

		/* RE-FLASH Timeout			*/
		case D_SYS_MSGID_RE_FLASH_TIMEOUT_NTC:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FILE_RW_TO;
			break;

		case D_API_MSGID_FILE_FIRM_READ_REQ:		/* API Firm Read Request	*/
		case D_SYS_MSGID_FLASH_RE_READ_RSP:			/* RE-FLASH Read Response	*/
			return;

		default:
			/* Assert */
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR,"Unknown Event 0x%x", f_fmngw_extEventNo);
			return;
	}

	/* Assert(EventNo&ThreadState) */
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "ThreadState:%d EventNo:%d", f_fmngw_thrdState, uiIntEventNo);
	
	/************************************************************/
	/* Call the corresponding function in matrix				*/
	/************************************************************/
	(*(f_fmngr_matrix[f_fmngw_thrdState][uiIntEventNo]))( pRcvMsg );
	
	return;    
}


/*!
 *  @brief  Analyze and dispatch message for RE-FLASH Control
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2015/08/14 TDIPS)sasaki create
 *  @date 2015/08/19 TDIPS)sasaki add D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP case
 *  @date 2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.65対応
 *  @date 2015/08/20 TDIPS)sasaki MKレビュー指摘事項No.61対応
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/

VOID f_fmng_anlz_frwre( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT 				uiIntEventNo;
	T_SYS_COMMON_THDIF 	*ptTskif;

	/************************************************************/
	/* Get event number from task IF header of message			*/
	/************************************************************/
	ptTskif = (T_SYS_COMMON_THDIF*)pRcvMsg;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	switch(ptTskif->uiEventNo)
	{
		/* API Firm Read Request	*/
		case D_API_MSGID_FILE_FIRM_READ_REQ:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FIRM_READ_REQ;
			break;

		/* API Firm Updata Request	*/
		case D_API_MSGID_FILE_FIRM_UPDATE_REQ:
			if(((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data.fw_type != D_API_FILE_FW_TYPE_RE)
			{
				return;
			}
			uiIntEventNo = D_FMNG_EVTIDX_RE_FIRM_SAVE_REQ;
			break;

		/* API Cancel Update Firm	*/
		case D_API_MSGID_FILE_FIRM_CANCEL_IND:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FIRM_CANCEL_IND;
			break;

		/* RE-FLASH Read Response		*/
		case D_SYS_MSGID_FLASH_RE_READ_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FIRM_READ_RSP;
			break;

		/* RE-FLASH Write Response		*/
		case D_SYS_MSGID_FLASH_RE_WRITE_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FIRM_WRITE_RSP;
			break;

		/* RE-FLASH Erase Response		*/
		case D_SYS_MSGID_FLASH_RE_ERASE_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FIRM_ERASE_RSP;
			break;

		/* FW Info Save Response		*/
		case D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FWINFO_SAVE_RSP;
			break;

		/* RE-FLASH Timeout				*/
		case D_SYS_MSGID_RE_FLASH_TIMEOUT_NTC:
			uiIntEventNo = D_FMNG_EVTIDX_RE_FLASH_ACCESS_TO;
			break;

		case D_API_MSGID_FILE_FIRM_ERASE_REQ:		/* API Firm Erase Request	*/
		case D_API_MSGID_FILE_FIRM_VERSION_REQ:		/* API Get Firm Version 	*/
		case D_SYS_MSGID_FLASH_ERASE_RSP:			/* Flash Erase Response		*/
		case D_SYS_MSGID_FLASH_WRITE_RSP:			/* Flash Write Response		*/
		case D_SYS_MSGID_TARPROCESS_RSP:			/* Tar Process Response		*/
		case D_SYS_MSGID_FLASH_TIMEOUT_NTC:			/* Timeout notification		*/
			return;

		default:
			/* Assert */
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR,"Unknown Event 0x%x", ptTskif->uiEventNo);
			return;
	}

	/* Assert(EventNo&ThreadState) */
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "RE-FLASH Control State:%d EventNo:%d", f_fmngw_re_thrdState, uiIntEventNo);
	
	/************************************************************/
	/* Call the corresponding function in matrix				*/
	/************************************************************/
	(*(f_fmngr_re_matrix[f_fmngw_re_thrdState][uiIntEventNo]))( pRcvMsg );
	
	return;    
}

/*! @} */
