/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_simuStop.c
 *  @brief  stop the simulator of maker MT 
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L2_MAIN
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "rrhApi_Dbg.h"

extern UINT f_recv_delMsgMap(UINT , UINT , UINT );


/********************************************************************************************************************/
/*!
 *  @brief  stop Maker MT.
 *  @note   stop Maker MT for this task.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)niemsh Create 
 *  @date   2015/8/11 TDIPS)ikeda rev.28026 ���r���[���f(38,39)�B��
 */
/********************************************************************************************************************/
VOID l2_dbg_simuStop(UINT *inf_p)
{
	L2_L3_TEST_NOTI 			   l3_test_noti;
	T_API_DBG_CPRISIMU_STOP_RSP  simursp;
	T_API_DBG_CPRISIMU_STOP_REQ *simureq_p;	/* Pointer of Maker MT stop req IF	*/
	UINT			linkno;
	UINT			result;
	UINT			copyLen;
	CHAR			errMsg[256];
	INT             loop;

	simureq_p = (T_API_DBG_CPRISIMU_STOP_REQ *)inf_p;
	linkno = simureq_p->data.linkno;

	result = CMD_OK;
	memset(errMsg, 0, sizeof(errMsg));

	/* stop simulator of this link	*/
	l2_dbgw_mkmtmode[linkno].simulator = D_API_DBG_SIMU_MODE_OFF;
	l2_dbgw_mkmtmode[linkno].rspQID_simu= 0;

	/* send l3 test stop notice to lpb thread */
	memset(&l3_test_noti, 0, sizeof(l3_test_noti));	
	l3_test_noti.header.uiEventNo		= D_L2_EVTNO_L3_TEST_STOP_NOTI;		/**< Event	ID		   */
	l3_test_noti.header.uiSignalkind	= 0;								/**< Signal Kind	   */
	l3_test_noti.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/**< Destination P QID */
		l3_test_noti.header.uiDstTQueueID	= CMD_TSKID_LPB;			/**< Destination T QID */
	l3_test_noti.header.uiSrcPQueueID	= D_RRH_PROCQUE_L2;				/**< Source P QID	   */
		l3_test_noti.header.uiSrcTQueueID	= CMD_TSKID_DBG;					/**< Source T QID	   */
	l3_test_noti.header.uiLength		= sizeof(l3_test_noti);				/**< Length 		   */
	l3_test_noti.link_num = simureq_p->data.linkno;

	l2_com_comMsgSnd(
				CMD_TSKID_LPB,						/* Thread queue ID		   */
			D_L2_EVTNO_L3_TEST_STOP_NOTI,		/* message ID*			   */
			(VOID *)&l3_test_noti,				/* The message to transmit */
			sizeof(l3_test_noti)				/* The size of message	   */
		);

	/* send response of startup cpri simulator */
	memset(&simursp, 0, sizeof(simursp));
	simursp.header.uiEventNo		= D_API_MSGID_DBG_CPRISIMU_STOP_RSP;	/**< Event	ID		   */
	simursp.header.uiSignalkind		= 0;									/**< Signal Kind	   */
	simursp.header.uiDstPQueueID	= simureq_p->header.uiSrcPQueueID;		/**< Destination P QID */
	simursp.header.uiDstTQueueID	= 0;									/**< Destination T QID */
	simursp.header.uiSrcPQueueID	= D_RRH_PROCQUE_L2;					/**< Source P QID	   */
	simursp.header.uiSrcTQueueID	= CMD_TSKID_INI;					/**< Source T QID	   */
	simursp.header.uiLength		= sizeof(simursp);							/**< Length 		   */
	simursp.data.result = result;
	copyLen = (strlen((CHAR *)errMsg) + 1) > sizeof(simursp.data.errMsg) ? sizeof(simursp.data.errMsg) : (strlen((CHAR *)errMsg) + 1);
	memcpy(simursp.data.errMsg, errMsg, copyLen);

	/* すべてのCPRIリンクについてシミュレータ停止している場合に振り分けテーブルから削除する */
	for (loop = D_RRH_CPRINO_REC; loop < D_RRH_CPRINO_NUM; loop++) {
	    if (l2_dbgw_mkmtmode[loop].simulator != D_API_DBG_SIMU_MODE_OFF) {
	        break;
	    }
	}

	if (D_RRH_CPRINO_NUM == loop) {
	    f_recv_delMsgMap(CMD_TSKIF_L1DATCNF, 0, CMD_TSKID_LPB);
	}

	/* call MW func */
	l2_com_comMsgSnd(
			simureq_p->header.uiSrcPQueueID,		/* Process queue ID 		*/
			D_API_MSGID_DBG_CPRISIMU_STOP_RSP,		/* message ID*			   */
			(VOID *)&simursp,						/* The message to transmit */
			sizeof(simursp)							/* The size of message	   */
		);

	return;
}


/* @} */

