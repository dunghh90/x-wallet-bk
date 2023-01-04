/*!
 * @skip  $ld:$
 * @file  f_cpr_anlz.c
 * @brief Analyze and dispatch message.
 * @date 2013/11/14 FFCS)niemsh create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "rrhApi_Cpri.h"
    
/*!
 * @addtogroup RRH_PF_CPR
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
 *  @date 2013/11/14 FFCS)niemsh create
 *  @warning	N/A
 *  @FeatureID	PF_Cpri-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 *  @date   2015/08/13 TDIPS)ikeda 「CPRI Linkup T.O」追加
 *  @date   2015/11/04 TDI)satou その他未実装-008 「遅延補正要求」追加
 *  @date   2015/11/10 TDI)satou その他未実装-017 「遅延補正要求(Step10)」追加
 */
VOID f_cpr_anlz( UINT *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT uiIntEventNo;
	T_SYS_COMMON_THDIF *ptTskif;
	
	/************************************************************/
	/* Get event number from task IF header of message			*/
	/************************************************************/
	ptTskif = (T_SYS_COMMON_THDIF*)pRcvMsg;
	f_cprw_extEventNo = ptTskif->uiEventNo;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	switch(f_cprw_extEventNo)
	{
		/*cpri link start indication */
		case D_API_MSGID_CPRI_LINK_STARTUP_IND:
			uiIntEventNo = D_CPR_EVT_CPRILNK_STARTUP_IND;
			break;

		/*cpri link enble indication */
		case D_API_MSGID_CPRI_LINK_ENABLE_IND:
			uiIntEventNo = D_CPR_EVT_CPRILNK_ENALBE_IND;
			break;

		/*cpri link disble indication */
		case D_API_MSGID_CPRI_LINK_DISABLE_IND:
			uiIntEventNo = D_CPR_EVT_CPRILNK_DISABLE_IND;
			break;

		/*CPRI link system param updating*/
		case D_API_MSGID_CPRI_PARAM_UPDATE_IND:
			uiIntEventNo = D_CPR_EVT_SYSTEMPARM_UPDATE_IND;
			break;

		/*CPRI state change IRQ notice*/
		case D_SYS_MSGID_CPRISTACHGNTC:
			uiIntEventNo = D_CPR_EVT_CPRILNK_STATE_CHG_NTC;
			break;

		/*The subscription of receving CPRI State*/
		case D_API_MSGID_CPRI_STATE_SUB_REQ:
			uiIntEventNo = D_CPR_EVT_CPRILNK_STATE_SUB_REQ;
			break;

		/*The unsubscription of receving CPRI State*/
		case D_API_MSGID_CPRI_STATE_UNSUB_IND:
			uiIntEventNo = D_CPR_EVT_CPRILNK_STATE_UNSUB_IND;
			break;

		/*CPRI  Link fiber output request*/
		case D_API_MSGID_CPRI_FIBEROUTPU_IND:
			uiIntEventNo = D_CPR_EVT_CPRILNK_FIBER_OUTPUT_IND;
			break;

		/*CPRI autonego complete notification */
		case D_SYS_MSGID_AUTONEGO_COMPLETE_NTC:
			uiIntEventNo = D_CPR_EVT_AUTONEGO_COMPLETE_NTC;
			break;

		/* CPRI Link Setup debug */
		case D_API_MSGID_CPRI_LINK_SETUP_DEBUG:
			f_cpr_dbg_link_setup(pRcvMsg);
			return;
			break;

		/* CPRI Linkup T.O */
		case D_SYS_MSGID_LINKUP_TIMEOUT_NTC:
			uiIntEventNo = D_CPR_EVT_LINKUP_TIMEOUT_NTC;
			break;

		/* CPRI Link Stop Indication */
		case D_API_MSGID_CPRI_LINK_STOP_IND:
		    uiIntEventNo = D_CPR_EVT_CPRILNK_STOP_IND;
		    break;

        /* 遅延補正要求(Step3) */
		case D_API_MSGID_CPRI_DLY_ADJ_STEP3_REQ:
		    uiIntEventNo = D_CPR_EVT_DELAY_ADJUST_STEP3_REQ;
		    break;

        /* 遅延補正要求(Step7) */
		case D_API_MSGID_CPRI_DLY_ADJ_STEP7_REQ:
		    uiIntEventNo = D_CPR_EVT_DELAY_ADJUST_STEP7_REQ;
		    break;

		/* 遅延補正要求(Step10) */
		// case D_API_MSGID_CPRI_DLY_ADJ_STEP10_REQ:
		//     uiIntEventNo = D_CPR_EVT_DELAY_ADJUST_STEP10_REQ;
		//     break;

		/* 遅延補正状態クリア要求 */
		case D_API_MSGID_CPRI_DLY_ADJ_CLEAR_REQ:
			uiIntEventNo = D_CPR_EVT_DELAY_ADJUST_CLEAR_REQ;
			break;

		/*CPRI link system param updating*/
		case D_API_MSGID_CPRI_CPMPOWERON_IND:
			uiIntEventNo = D_CPR_EVT_CPMPOWERON_IND;
			break;

		default:
			f_com_assert(f_cprw_extEventNo,"pf_cpr recv unexpected msg");
			return;
	}
	
	/************************************************************/
	/* Call the corresponding function in matrix				*/
	/************************************************************/
	(*(f_cprr_matrix[f_cprw_thrdState][uiIntEventNo]))( pRcvMsg );
	
	return;    
}

/*! @} */
