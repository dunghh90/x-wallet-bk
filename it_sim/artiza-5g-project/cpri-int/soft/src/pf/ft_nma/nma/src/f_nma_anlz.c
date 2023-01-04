/*!
 * @skip  $ld:$
 * @file  f_nma_anlz.c
 * @brief Analyze and dispatch message.
 * @date  2013/03/25 FFCS)Xutong Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_nma_inc.h"
    
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
 *  @date  2013/11/13 FFCS)linb Create\n
 *  @date  2018/08/27 KCN)若水  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nma_anlz( VOID *pRcvMsg )
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
		/* EventLog Write Request(Clear/Write)(from pf_log) */
		case D_SYS_MSGID_FLSLOGWR_IND:
			evtIdx = D_NMA_EVTIDX_FLSHLOGWR_IND;
			break;
		/* TroubleLog Write Request(from pf_lgc) */
		case D_SYS_MSGID_TROUBLE_LOG_WRITE_REQ:
			evtIdx = D_NMA_EVTIDX_TRBLOGLW_REQ;
			break;
		/* TroubleLog Write Request(from pf_main) */
		case D_SYS_MSGID_TROUBLELOG_WRITE_REQ:
			evtIdx = D_NMA_EVTIDX_TRBLOGMW_REQ;
			break;
		/* Install File Write Request(form pf_flm) */
		case	D_SYS_MSGID_NMA_FILE_WRITE_REQ:
			evtIdx = D_NMA_EVTIDX_FLSHFILWR_REQ;
			break;
		/* NetConf Data Write Request(from pf_fhs) */
		case D_SYS_MSGID_NMA_NETCONF_WRITE_REQ:
			evtIdx = D_NMA_EVTIDX_NETCONFW_REQ;
			break;
		/* VLAN Data Write Request(from ft_dhcp) */
		case D_SYS_MSGID_NMA_VLAN_WRITE_REQ:
			evtIdx = D_NMA_EVTIDX_VLANWRT_REQ;
			break;
		default:
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR , "Unknown Event:0x%08x",ptThrdif->uiEventNo);
			return;
	}
	
    /************************************************************/
    /* Call the corresponding function in matrix                */
    /************************************************************/
    f_nmar_matrix[f_nmaw_thrState][evtIdx]( pRcvMsg );
    
	return;    
}

/* @} */
