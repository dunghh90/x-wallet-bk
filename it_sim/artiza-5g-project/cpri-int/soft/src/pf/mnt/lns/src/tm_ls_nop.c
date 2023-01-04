/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_nop.c
 *  @brief   no operation
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_ls_header.h"			/* LNS task header					*/

/********************************************************************************************************************/
/**
 *  @brief     no operation
 *  @note     Free the received message if it is not NULL
 *  @param  a_inf_p  	[in] point of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

void tm_ls_Nop(UINT * a_inf_p)
{
	/*Free the received message if it is not NULL*/
	UINT a_ret;
	CMT_TSKIF_TCPSNDREQ	*a_tcpsndreq_p;

	cm_Assert( CMD_ASL_DBGLOW, CMD_NUM0, "tm_ls_Nop is called" );

    a_tcpsndreq_p = (CMT_TSKIF_TCPSNDREQ*)a_inf_p;
    
    /*	要因がTCP データ送信通知の場合	*/
    if(a_tcpsndreq_p->head.uiEventNo == CMD_TSKIF_TCPSNDREQ) /* pgr0689 */
    {
		if(a_tcpsndreq_p->msgbody != NULL)
		{
			a_ret = cm_BFree(a_tcpsndreq_p->msgbody);
			if(a_ret != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL,"tm_ls_Nop", 
							"cm_BFree:NG",
							a_ret,(UINT)a_tcpsndreq_p->msgbody,a_tcpsndreq_p->head.uiEventNo);
			}
		}
	}
	if(a_inf_p){
		a_ret = cm_BFree((VOID*)a_inf_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL,"tm_ls_Nop", 
						"cm_BFree:NG",
						a_ret,(UINT)a_inf_p,CMD_NUM0);
		}
	}
}
/* @} */
