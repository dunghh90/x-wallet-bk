/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_TcpRel.c
 *  @brief   Disconnect the specified TCP link
 *  @date   2008/07/28  FFCS)zhengmh Create
 *  @date   2013/11/22  FFCS)zhaodx modify for zynq
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
#include <sys/types.h>
#include <linux/socket.h>

/********************************************************************************************************************/
/**
 *  @brief     Disconnect the specified TCP link
 *  @note     Disconnect the specified TCP link
 *  @param  a_inf_p 	[in]	point of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

VOID tm_ls_TcpRel(UINT * a_inf_p)
{
	INT						a_ret;/*information returned by function*/
	INT						a_sockethandle;/*socket handle*/
	CMT_TSKIF_TCPRELNTC*	a_relntc_p;/*Buffer for task IF*/

	/*Type conversion*/
	a_relntc_p = (CMT_TSKIF_TCPRELNTC*)a_inf_p;
	a_sockethandle = a_relntc_p->linkhandle;/*Get socket handle*/

	/*Call socket function to disconnect tcp link*/
	a_ret = shutdown(a_sockethandle,2);
	cm_MWait(10);
	a_ret = close(a_sockethandle);
	/*Fail to disconnect, record the returned error info  */
	if(a_ret != SOCK_RES_OK)
	{
		lsw_errinfo.ls_close = a_ret;
	}

	/*Free buffer for task IF*/
	a_ret = (INT)cm_BFree(a_inf_p);
	if(a_ret != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpRel",
					  "Free buffer for taskIF  NG",(UINT) a_ret, (UINT)a_inf_p, CMD_NUM0);
	}
}
 /* @} */
