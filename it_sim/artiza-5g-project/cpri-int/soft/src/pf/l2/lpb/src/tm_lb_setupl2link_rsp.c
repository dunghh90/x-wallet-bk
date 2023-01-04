/*!
 * @skip    $Id$
 * @file    tm_lb_setupl2link_rsp.c
 * @brief   Send response of setup lapb link
 *  @date   2013/11/21 FFCS)honjg Create for zynq
 * 
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"			/* TRIF common head file		       	*/
#include "l2_com_inc.h"
#include "rrhApi_L2.h"
#include "tm_lb_def.h"
#include "tm_lb_typ.h"
#include "tm_lb_ext.h"
#include "m_lb_def.h"
#include "m_lb_typ.h"
#include "m_lb_ext.h"



/********************************************************************************************************************/
/*!
 *  @brief  Send response of setup lapb link
 *  @note   Send response of setup lapb link to subscription process
 *  @param  linkno   [in]   CPRI link number
 *  @param  result   [in]   link state
 *  @param  errMsg   [in]   error message
 *  @retval -
 *  @date   2013/11/21 FFCS)hongj Create for zynq
 */
 /********************************************************************************************************************/
void 	tm_lb_setupl2link_rsp(USHORT linkno, UINT result, CHAR* errMsg)
{
	T_API_L2LINK_SETUP_RSP apiRsp;
	UINT copyLen;

	/* send response of setup lapb link */
	memset(&apiRsp, 0, sizeof(apiRsp));

    if(lbw_lnk_set_respQId[linkno] == CMD_NUM0)
    {
        cm_Assert( CMD_ASL_DBGLOW, lbw_lnk_set_respQId[linkno],
			    "[tm_lb_setupl2link_rsp] There isn't setup response queue Id." );
		return;
    }
	apiRsp.header.uiEventNo		= D_API_MSGID_L2_LINK_SETUP_RSP;	/**< Event	ID		   */
	apiRsp.header.uiSignalkind	= 0;								/**< Signal Kind	   */
	apiRsp.header.uiDstPQueueID	= lbw_lnk_set_respQId[linkno];				/**< Destination P QID */
	apiRsp.header.uiDstTQueueID	= 0;							/**< Destination T QID */
	apiRsp.header.uiSrcPQueueID	= D_RRH_PROCQUE_L2;				/**< Source P QID	   */
	apiRsp.header.uiSrcTQueueID	= D_L2_MSGQ_LPB;					/**< Source T QID	   */
	apiRsp.header.uiLength		= sizeof(apiRsp);					/**< Length 		   */
	apiRsp.data.result = result;
	copyLen = (strlen((CHAR *)errMsg) + 1) > sizeof(apiRsp.data.errMsg) ? sizeof(apiRsp.data.errMsg) : (strlen((CHAR *)errMsg) + 1);
	memcpy(apiRsp.data.errMsg, errMsg, copyLen);

	tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, CMD_TSKID_CPR, D_API_MSGID_L2_LINK_SETUP_RSP, (UINT *)&apiRsp);
	/* call send message func */
	l2_com_comMsgSnd(
			lbw_lnk_set_respQId[linkno],						/* Thread queue ID		   */
			D_API_MSGID_L2_LINK_SETUP_RSP,
			(VOID *)&apiRsp,				/* The message to transmit */
			sizeof(apiRsp)					/* The size of message	   */
		);

	lbw_lnk_set_respQId[linkno] = CMD_NUM0;
	
	return;
}


/* @} */

