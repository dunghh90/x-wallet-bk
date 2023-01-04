/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_Main.c
 *  @brief   Call event-processing function, according to received event 
 *  @date   2008/07/25 FFCS)zhengmh create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

																		
#include "m_cm_header.h"			/*   TRA  common head file				*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/


/********************************************************************************************************************/
/**
 *  @brief     Call event-processing function, according to received event 
 *  @note     Call event-processing function, according to received event 
 *  @param  source	[in]	Source task
 *  @param  event		[in]	Event no
 *  @param  inf_p		[in]	Pointer of received buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/



VOID m_mk_Main(UINT source, UINT event, UINT *inf_p)
{
	UINT 		ret;		/* Return value of event analyze	*/
	UINT		ret_bfree;	/* Return value of  buffer free		*/
	
	ret = MKD_RET_ABNORMAL;
	ret_bfree = CMD_NUM0;
	
	/* Get internal event no			*/
	ret = m_mk_Anlz(source, event);

	if(ret == MKD_RET_NORMAL)
	{
         (*(mkr_mtrxtbl[mkw_stateno][mkw_eventno]))(inf_p);	/* Matrix table process*/
	}
	else
	{
		cm_Assert( CMD_ASL_DBGLOW, event, "[m_mk_Main] Event not found" );
	}
	
	if(mkw_needfrmsg && inf_p)		/* If need free msg buffer and pointer not null	*/
	{
		ret_bfree = cm_BFree(inf_p);/* Free message buffer				*/	

		if(ret_bfree != CMD_RES_OK)
		{
			cm_MAbort( CMD_ALMCD_BUFREL, 
						"m_mk_Main", 
					   	"cm_BFree NG", 
					 	  ret_bfree, 
					 	  (UINT)inf_p, 
					 	  CMD_NUM0);			
		}
	}
	
	mkw_needfrmsg = MKD_TRUE;	/* Need to free message buffer	*/
	
	return;						/* Task stop					*/

}

VOID m_mk_logPrint(FILE *outf)
{
	fprintf(outf,"mk communcation----------------------------------------\n");
	fprintf(outf," simu rec     %d,%d\n",
	        cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].conn,
			cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].tcplinkhdl);
    fprintf(outf," simu re01_06 %d,%d\n",
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].conn,
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl);
    fprintf(outf," simu re07_12 %d,%d\n",
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].conn,
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl);
    fprintf(outf," simu re13_16 %d,%d\n",
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].conn,          /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl);   /* pgr0012 pgr0860 */
	fprintf(outf," moni rec     %d,%d\n",
	        cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].conn,              /* pgr0012 pgr0860 */
			cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].tcplinkhdl);       /* pgr0012 pgr0860 */
    fprintf(outf," moni re01_06 %d,%d\n",
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].conn,          /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl);   /* pgr0012 pgr0860 */
    fprintf(outf," moni re07_12 %d,%d\n",
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].conn,          /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl);   /* pgr0012 pgr0860 */
    fprintf(outf," moni re13_16 %d,%d\n",
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].conn,          /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].tcplinkhdl);   /* pgr0012 pgr0860 */
	fprintf(outf," fhmmt        %d,%d\n",
	        cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].conn,                  /* pgr0012 pgr0860 */
			cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].tcplinkhdl);           /* pgr0012 pgr0860 */

	return;
}
/* @} */
