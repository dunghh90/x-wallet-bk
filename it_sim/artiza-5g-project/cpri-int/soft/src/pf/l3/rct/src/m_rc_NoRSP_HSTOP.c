/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_NoRSP_HSTOP.c
 *  @brief  
 *  @date 2015/09/02 TDIPS)maruyama create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 
 */
/********************************************************************************************************************/

/** @addtogroup L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief  要求に対して応答のないREは強制停止
 *  @param  resync_tgt_re   [in] ReSync対象RE  
 *  @param  pCpriState      [in] CPRI state table
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01988
 *  @date 2015/09/02 TDIPS)maruyama create
 *  @date 2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 *  @date 2015/11/09 TDIPS)sasaki IT2問処No.162対処
 *  @date 2015/11/13 TDIPS)sasaki M-RRU-ZSYS-01988 IT2問処No.181対処 強制停止条件変更
 *  @date 2015/11/14 TDIPS)sasaki IT2問処No.186対処
 *  @date 2015/12/03 TDIPS)sasaki IT3問処No.113対処
 */
/********************************************************************************************************************/

VOID m_rc_NoRSP_HSTOP(USHORT *resync_tgt_re, T_RRH_CPRI_STATE *pCpriState)
{
	UINT				cnt;

	for(cnt = D_RRH_CPRINO_RE_MIN; cnt <= D_RRH_CPRINO_RE_MAX; cnt++)
	{	
		if((resync_tgt_re[cnt-1] == D_RRH_OFF) && (pCpriState->cprista_re[cnt-1] == D_RRH_CPRISTAT_F))
		{
			/* 3G */
			if((re_startupREQ[cnt][CMD_SYS_3G] == CMD_ON) && (re_startupRSP[cnt][CMD_SYS_3G] == CMD_OFF))
			{
				/* 要求したけど応答がない */
				/* 強制停止	*/
				m_rc_ReHCStop(cnt, CMD_SYS_3G, E_RRH_RESTOP_FACTOR_OTHER);
			}
			/* LTE */
			if((re_startupREQ[cnt][CMD_SYS_S3G] == CMD_ON) && (re_startupRSP[cnt][CMD_SYS_S3G] == CMD_OFF))
			{
				/* 要求したけど応答がない */
				/* 強制停止	*/
				m_rc_ReHCStop(cnt, CMD_SYS_S3G, E_RRH_RESTOP_FACTOR_OTHER);
			}
		}
	}
}
/* @} */

