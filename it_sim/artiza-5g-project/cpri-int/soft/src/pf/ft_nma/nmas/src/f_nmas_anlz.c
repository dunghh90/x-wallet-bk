/*!
 * @skip  $ld:$
 * @file  f_nmas_anlz.c
 * @brief Analyze and dispatch message.
 * @date  2013/03/25 FFCS)Xutong Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_nmas_inc.h"
    
/*!
 * @addtogroup RRH_PF_NMA
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
 *  @date  2019/02/21 KCN)kamimoto  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nmas_anlz( VOID *pRcvMsg )
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
    if( ptThrdif->uiEventNo == D_SYS_MSGID_NMAS_FILE_WRITE_REQ )
    {
		evtIdx = D_NMAS_EVTIDX_FLSHFILWR_REQ;
	}
	else if(ptThrdif->uiEventNo == D_SYS_MSGID_NMAS_LUTLOG_WRITE_NFT){
		evtIdx = D_NMAS_EVTIDX_LUTLOGWRITE_NTF;
	}
	else
	{
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR , "Unknown Event:0x%08x",ptThrdif->uiEventNo);
		return;
	}
	
    /************************************************************/
    /* Call the corresponding function in matrix                */
    /************************************************************/
    f_nmasr_matrix[f_nmasw_thrState][evtIdx]( pRcvMsg );
    
	return;    
}

/* @} */
