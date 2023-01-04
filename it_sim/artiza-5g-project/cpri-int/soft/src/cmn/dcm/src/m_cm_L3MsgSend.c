/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_L3MsgSend.c
 *  @brief  common function - L3 message sending  function
 *  @date   2008/07/18 FFCS)Tangj create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/***************************************************************************/

/** @addtogroup RRH_DCM
 *  @{
 */

#include "m_cm_header.h" /*	TRA common task header file */
#include "rrhApi_L2.h"
/**************************************************************************/
/**
 *  @brief  common function - L3 message sending  function
 *  @note   1. for some message, which is sent under L3 ok condition,\n
 *             check L3 state\n
 *          2. for some message, which it sent under stateF condition,\n
 *             check stateF\n
 *          3. ENTER L3 message to LAPB\n
 *  @param  linkno			[in]	cpri link number
 *  @param  cprimsgbuff_p	[in]	L3 message info point
 *  @param  file			[in]	calling function file name
 *  @param  line			[in]	calling function executing line
 *  @return RETURN CODE
 *  @retval CMD_L3_OK			0: 正常
 *  @retval CMD_L3_CPRICUT		1: CPRI link down
 *  @retval CMD_RES_PARAMERR	0xF0000008: Parameter specify error
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2013/12/03 FFCS)niemsh modify for zynq 
 *  @date   2015/7/29 TDIPS)ikeda rev.27602 m_cm_L3MsgSend対応
 *  @date   2015/7/30 TDIPS)ikeda rev.27665 L3 17リンク対応
 *  @date   2015/8/3 TDIPS)ikeda rev.27733 17リンク対応、対RE実装

 */
/**************************************************************************/

UINT m_cmR_L3MsgSend( USHORT linkno, void* cprimsgbuff_p, VOID *file, UINT line)
{
	UINT returncode;	/* function call return code */

	if( cprimsgbuff_p == NULL )
	{
		/* Message Buffer point is zero , */
		return(CMD_RES_PARAMERR);
	}

	/* get the system type from signal_kind */
	/* set info to buffer */
	( (CMT_TSKIF_CPRISND_SIGNALGET*)cprimsgbuff_p) ->cprisnd_inf.link_num = linkno;
	( (CMT_TSKIF_CPRISND_SIGNALGET*)cprimsgbuff_p) ->cprisnd_inf.length = 
	( (CMT_TSKIF_HEAD *)cprimsgbuff_p) ->uiLength - sizeof(CMT_CPRISIGSND_INF) - sizeof(CMT_TSKIF_HEAD); 

    /* send L3 message to LAPB task */
	returncode = rrhApi_L2_Mnt_SendCpriMsg((INT)(((CMT_TSKIF_HEAD*)cprimsgbuff_p)->uiDstPQueueID),
				0,0,cprimsgbuff_p);
	if(returncode != E_API_RCD_OK)
	{
		return CMD_L3_API_NG;
	}

	return(CMD_L3_OK);
}
/* @} */
