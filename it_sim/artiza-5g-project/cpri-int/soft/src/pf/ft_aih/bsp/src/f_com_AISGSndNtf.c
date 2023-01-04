/*!
 * @skip  $ld:$
 * @file  f_com_AISGSndNtf.c
 * @brief Send AISG data sending complete notification to AL2 task.
 * @date  2010/10/10 FFCS)Zouw create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010
 */
 
/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_com_inc.h"

/*!
 *  @brief  Send AISG data sending complete notification to AL2 task.
 *          - this function is called by AISG interrupt.
 *  @note   This function is processed as follows.\n
 *              - buffer allocate
 *              - fill message head
 *              -send L1 data sending complete notification to AL2 task
 *  @param  none
 *  @return VOID
 *  @retval none  
 *  @date   2010/10/10 FFCS)Zouw create
 */
void f_com_AISGSndNtf() 
{	
	T_SYS_AISG_SNDCMP_NTC  Sndcmpntc;  /* sending data complete notification table */

	
	/***********************************/
	/*init message head */
	/***********************************/
	memset(&Sndcmpntc,0,sizeof(Sndcmpntc));
	Sndcmpntc.thdIf.uiEventNo = D_SYS_MSGID_AIH_ALDSND_END;
	Sndcmpntc.thdIf.uiLength = sizeof(T_SYS_AISG_SNDCMP_NTC);
	Sndcmpntc.thdIf.uiSignalkind = D_DU_IFLOG_OFF;	/* ÉçÉOoff */

	/*************************/
	/*send a message */
	/************************/
	f_com_msgQSend(
				D_SYS_THDQID_PF_AIH,
				(CHAR*)&Sndcmpntc,
				sizeof(T_SYS_AISG_SNDCMP_NTC),
				D_SYS_THDQID_PF_AIH,
				D_SYS_ALLF_SHORT,
				D_SYS_ALLF_SHORT
				);
	
	return;

}
/* @} */

