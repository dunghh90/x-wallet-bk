/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriStateSubProc.c
 *  @brief  The subscription of receving CPRI State message function
 *  @date 2013/11/14 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "rrhApi_Cpri.h"

/**
* @brief 	The subscription of receving CPRI State message function
* @note  	The subscription of receving CPRI State message function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 		2013/11/14 FFCS)niemsh create
* @date     2015/7/28 TDIPS)ikeda rev.27478 CPRIリンク番号複数対応
* @warning	N/A
* @FeatureID	PF_Cpri-002-003-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriStateSubProc(UINT * buff_adr)
{
	T_API_CPRILINK_STATE_SUB_REQ *apiSub_pt;
	UINT 						num;
	INT						blank;
	USHORT					a_link_num;

	/* 
	update f_cprw_cprStatChgNtc_QID if the qid is not found
	*/
	apiSub_pt = (T_API_CPRILINK_STATE_SUB_REQ*)buff_adr ;
	a_link_num = apiSub_pt->link_num;
	blank = -1;
	for(num = 0; num < sizeof(f_cprw_cprStatChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_cprw_cprStatChgNtc_QID[ num] == apiSub_pt->header.uiSrcPQueueID)
		{
			break;
		}
		if((f_cprw_cprStatChgNtc_QID[ num] == 0) && (blank < 0))
		{
			blank = num;
		}
	}

	if(num >= sizeof(f_cprw_cprStatChgNtc_QID) / sizeof(UINT))
	{
		if(blank >= 0)
		{
		    f_cprw_cprStatChgNtc_QID[ blank] = apiSub_pt->header.uiSrcPQueueID;
		}
		else 
		{
			f_com_assert(apiSub_pt->header.uiSrcPQueueID,"too many subscribe cpri state");
			return;
		}
	}

	f_cpr_com_CpriStateNtc( a_link_num, apiSub_pt->header.uiSrcPQueueID);
	
	return;
}


/**
* @brief The subscription of receving CPRI State message function
* @note  The subscription of receving CPRI State message function.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2013/11/14 FFCS)niemsh create
* @warning	N/A
* @FeatureID	PF_Cpri-002-003-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriStateUnsubProc(UINT * buff_adr)
{
	T_API_CPRILINK_STATE_UNSUB_IND *apiSub_pt;
	UINT 						num;

	apiSub_pt = (T_API_CPRILINK_STATE_UNSUB_IND*)buff_adr;
	/* 
	remove QID from f_cprw_cprStatChgNtc_QID 
	*/
	for(num = 0; num < sizeof(f_cprw_cprStatChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_cprw_cprStatChgNtc_QID[ num] == apiSub_pt->header.uiSrcPQueueID)
		{
			f_cprw_cprStatChgNtc_QID[ num]  = 0;
			break;
		}
	}

	return;
}
/**
* @brief The common function of sending current cpri state to APL
* @note  The common function of sending current cpri state to APL
* @param link_num [in] CPRI link number
* @param respQid  [in] response queue id
* @return None
* @date 2013/11/14 FFCS)niemsh create
* @date 2015/08/04 TDIPS)ikeda 2.4Gビットレート対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_cpr_com_CpriStateNtc(USHORT link_num, UINT respQid)
{
	T_API_CPRILINK_STATE_NTC		apiNtc;
	UINT						cpriStat;
	UINT						a_lapbstatus;		/* LAPB status register value */
	UINT						bChange = D_SYS_OFF;
	CHAR *		bitratename[] = { "2.4G","4.9G","9.8G"};

	/* read current cpri state */
	cpriStat = 0;
	memset(&apiNtc, 0, sizeof(apiNtc));
	
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
			M_RRH_REG_CPRI_CPSTAT(link_num), &cpriStat);

	/* the value in cpri state register is same as the defintion of cpri state*/
	apiNtc.cpriState = cpriStat;
	
	if(cpriStat < D_RRH_CPRISTAT_E)
	{
		apiNtc.cpriEDown = D_RRH_ON;

		bChange = D_SYS_ON;
	}

	if(cpriStat >= D_RRH_CPRISTAT_E)
	{
		/* Auto Nego Complete? */
		if(f_cprw_autonego_complete_flag == D_SYS_ON)
		{
			bChange = D_SYS_ON;
			apiNtc.cpriEDown = D_RRH_OFF;
			
			/* bit rate setting */
			apiNtc.cpriLineBitrate = f_cprw_bitrate_real;

			/* LAPB status register */
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
					M_RRH_REG_CM_LAXIST(link_num), &a_lapbstatus);
			
			/* ACT status */
			if(a_lapbstatus == D_SYS_NUM0)
			{
				apiNtc.cpriHdlc = D_RRH_ON;
			}


			/* bitrate 取得 */
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
					M_RRH_REG_CPRI_CPTXCW(link_num), &apiNtc.cpriHDLCBitrate);
			apiNtc.cpriHDLCBitrate &= D_RRH_HDLC_RATE_MASK;

			/* log */
			BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO, "L3 Start!Bit Rate = %s(%d)", 
										bitratename[apiNtc.cpriLineBitrate],
										apiNtc.cpriLineBitrate);
		}
	}

	if(bChange == D_SYS_ON)
	{
		apiNtc.header.uiEventNo = D_API_MSGID_CPRI_STATE_STATE_NTC;
		apiNtc.header.uiLength = sizeof(apiNtc);
		apiNtc.link_num = link_num;

		apiNtc.header.uiDstPQueueID = respQid;

		/* write running history */
		f_cpr_cm_runHisSet(D_CPR_SENDIEVEFLG, (UCHAR*)&apiNtc);

		/* send message to apl */
		f_com_sendMsgToAplFHM(respQid, &apiNtc, sizeof(apiNtc),
						apiNtc.header.uiEventNo, D_SYS_THDQID_PF_CPRI);
	}
	
	return;
}
/* @} */

