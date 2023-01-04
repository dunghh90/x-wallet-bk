/*!
 * @skip  $ld:$
 * @file  f_frw_anlz.c
 * @brief Analyze and dispatch message.
 * @date  2013/03/25 FFCS)Xutong Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_frw_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRW
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
 *  @date  2013/11/13 FFCS)linb Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frw_anlz( VOID *pRcvMsg )
{
    /************************************************************/
    /* Local parameter declaration                              */
    /************************************************************/
	UINT					evtIdx = D_SYS_NUM0;
    T_SYS_COMMON_THDIF	 	*ptThrdif = NULL;
	
    /************************************************************/
    /* Get event number from task IF header of message          */
    /************************************************************/
    ptThrdif = (T_SYS_COMMON_THDIF*)pRcvMsg;
	
    /************************************************************/
    /* Transform the event number to internal event number      */
    /************************************************************/
	switch(ptThrdif->uiEventNo)
	{
		/* Flash Erase Request(from pf_fmng) */
		case D_SYS_MSGID_FLASH_ERASE_REQ: 
			evtIdx = D_FRW_EVTIDX_FLSHERASE_REQ;
			break;

		/* Flash Write Request(from pf_fmng) */
		case D_SYS_MSGID_FLASH_WRITE_REQ:
			evtIdx = D_FRW_EVTIDX_FLSHWRITE_REQ;
			break;

		/* Flash Read Request(from pf_dia) */
		case D_SYS_MSGID_FLASH_READ_REQ:
			evtIdx = D_FRW_EVTIDX_FLSHREAD_REQ;
			break;

		/* EventLog Write Request(Clear/Write)(from pf_log) */
		case D_SYS_MSGID_FLSLOGWR_IND:
			evtIdx = D_FRW_EVTIDX_FLSHLOGWR_IND;
			break;
		
		/* TroubleLog Write Request(from pf_main) */
		case D_SYS_MSGID_TROUBLELOG_WRITE_REQ:
			evtIdx = D_FRW_EVTIDX_TRBLOGW_REQ;
			break;

		case D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ:
			evtIdx = D_FRW_EVTIDX_FWINFO_SAVE_REQ;
			break;
			
		default:
			cmn_com_assert_th_get(f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR , "Unknown Event:0x%08x",ptThrdif->uiEventNo);
			return;
	}
	
    /************************************************************/
    /* Call the corresponding function in matrix                */
    /************************************************************/
    f_frwr_matrix[f_frww_thrState][evtIdx]( pRcvMsg );
    
	return;    
}

/* @} */
