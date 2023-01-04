/*!
 * @skip  $ld:$
 * @file  f_nego_anlz.c
 * @brief Analyze and dispatch message.
 * @date 2015/04/22 FFCS)fuxg create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-
 */
#include "f_sys_inc.h"
#include "f_nego_inc.h"

/*!
 * @addtogroup RRH_PF_NEGO
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
 *  @date 2015/04/22 FFCS)fuxg create
 *  @warning	N/A
 *  @FeatureID	RRH-012-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nego_anlz( UINT *pRcvMsg )
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
	f_nego_extEventNo = ptTskif->uiEventNo;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	switch(f_nego_extEventNo)
	{
		/* auto nego start indication */
		case D_SYS_MSGID_AUTONEGO_START_IND:
			uiIntEventNo = D_NEGO_EVT_START_IND;

			/* write running history */
			f_nego_cm_runHisSet(D_NEGO_RECVIEVEFLG, (UCHAR*)pRcvMsg);
			break;
			
		/* auto nego stop indication */
		case D_SYS_MSGID_AUTONEGO_STOP_IND:
			uiIntEventNo = D_NEGO_EVT_STOP_IND;

			/* write running history */
			f_nego_cm_runHisSet(D_NEGO_RECVIEVEFLG, (UCHAR*)pRcvMsg);
			break;
			
		/* auto nego timeout notification */
		case D_SYS_MSGID_AUTONEGO_TIMEOUT_NTC:
			uiIntEventNo = D_NEGO_EVT_TIMEOUT_NTC;
			break;
			
		/* auto nego restart indication */
		case D_SYS_MSGID_AUTONEGO_RESTART_IND:
			uiIntEventNo = D_NEGO_EVT_RESTART_IND;

			/* write running history */
			f_nego_cm_runHisSet(D_NEGO_RECVIEVEFLG, (UCHAR*)pRcvMsg);
			break;

		/* Auto Nego Result Set for Debug */
		case D_API_MSGID_NEGO_BITRATERESULT_DBG:
			/* write running history */
			f_nego_cm_runHisSet(D_NEGO_RECVIEVEFLG, (UCHAR*)pRcvMsg);

			f_nego_dbg_bitrate_result(pRcvMsg);
			return;
			break;
			
		default:
			f_com_assert(f_nego_extEventNo,"pf_nego recv unexpected msg");
			return;
	}
	
	/************************************************************/
	/* Call the corresponding function in matrix				*/
	/************************************************************/
	(*(f_nego_matrix[f_nego_thrdState][uiIntEventNo]))( pRcvMsg );
	
	return;    
}

/*! @} */
