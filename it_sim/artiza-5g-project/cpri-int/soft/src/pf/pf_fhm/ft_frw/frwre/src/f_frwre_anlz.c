/*!
 * @skip  $ld:$
 * @file  f_frwre_anlz.c
 * @brief Analyze and dispatch message.
 * @date  2015/08/04 TDIPS)sasaki Create\n
 * @date  2015/08/27 TDIPS)sasaki Update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_frwre_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRWRE
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
 *  @date  2015/08/04 TDIPS)sasaki Create
 *  @date  2015/08/27 TDIPS)sasaki CT検出問題(caseラベルにFMNG-FRW間メッセージIDを使用していた)を改修
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwre_anlz( VOID *pRcvMsg )
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
		/* RE-Flash Erase Request(from pf_fmng) */
		case D_SYS_MSGID_FLASH_RE_ERASE_REQ: 
			evtIdx = D_FRWRE_EVTIDX_FLSHERASE_REQ;
			break;

		/* RE-Flash Write Request(from pf_fmng) */
		case D_SYS_MSGID_FLASH_RE_WRITE_REQ:
			evtIdx = D_FRWRE_EVTIDX_FLSHWRITE_REQ;
			break;

		/* RE-Flash Read Request(from pf_fmng) */
		case D_SYS_MSGID_FLASH_RE_READ_REQ:
			evtIdx = D_FRWRE_EVTIDX_FLSHREAD_REQ;
			break;
					
		default:
			cmn_com_assert_th_get(f_frwrew_assert_p, D_RRH_LOG_AST_LV_ERROR , "Unknown Event:0x%08x",ptThrdif->uiEventNo);
			return;
	}
	
    /************************************************************/
    /* Call the corresponding function in matrix                */
    /************************************************************/
    f_frwrer_matrix[f_frwrew_thrState][evtIdx]( pRcvMsg );
    
	return;
}


/* @} */
