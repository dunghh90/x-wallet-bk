/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_CpriStaPriorityChk.c
 *  @brief  CPRI state priority check function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理 task header file */

/* @{ */
/**
* @brief CPRI state priority check function
* @note check CPRI state priority between newest CPRI state and old state,
              and update CPRI state common management table.\n
* @param linkno [in] CPRIリンク番号 
* @param linkevent [in] the related event of link state 
* @param cpristate [in] the E-DOWN cpri state(State A/B/C/D)
* @param systemtype [in] the system type of L3 OK/DISC
* @param prioind [out] the event priority 
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2015/07/30 TDIPS)ikeda 17リンク対応
*
*/

VOID m_cr_CpriStaPriorityChk(USHORT linkno, USHORT linkevent, USHORT cpristate,
										USHORT systemtype, USHORT * prioind)
 {
	USHORT cprilnkstano;	/* CPRI link state no */
	USHORT newcprista;		/* input CPRI state */
	USHORT loopnum;			/* use as loop counter */

	if( prioind == NULL )
	{
		return;
	}
	
	/* get old CPRI link state number */
	if(( linkevent == CRD_LINKEV_L3HCOK ) || ( linkevent == CRD_LINKEV_L3HCDSC ))
	{
		cprilnkstano = cmw_cprilnkstatbl[linkno].cprilnkstano[systemtype];
	}
	else
	{
		cprilnkstano = cmw_cprilnkstatbl[linkno].cprilnkstano[CMD_NUM0];
	}

	/* confirm priority with old CPRI link state number and link event  */
	*prioind = crr_cprilnksta_priotbl[cprilnkstano][linkevent].priority;
	newcprista = crr_cprilnksta_priotbl[cprilnkstano][linkevent].newcprista;

	if (*prioind == CRD_STAPRIO_LOW)
	{  /* in case of  low priority, not need to update CPRI state management table */

		return;
	}


	/* update health check status for 3G and S3G seperatly */
	if(( linkevent == CRD_LINKEV_L3HCOK ) || ( linkevent == CRD_LINKEV_L3HCDSC ))
	{
		cmw_cprilnkstatbl[linkno].cprilnkstano[systemtype] = newcprista;
		cmw_cprilnkstatbl[linkno].hcsta[systemtype] = crr_cprilnksta_class_tbl[newcprista].hcsta;
	}
	else
	{
		for (loopnum = CMD_NUM0; loopnum < CMD_MAX_SYSNUM; loopnum++)
		{
			cmw_cprilnkstatbl[linkno].cprilnkstano[loopnum] = newcprista;
			cmw_cprilnkstatbl[linkno].hcsta[loopnum] = crr_cprilnksta_class_tbl[newcprista].hcsta;
		}
	}
	
	/* below processing is for same priority or high priority */
	cmw_cprilnkstatbl[linkno].cprista =	crr_cprilnksta_class_tbl[newcprista].cprista;
	cmw_cprilnkstatbl[linkno].hdlcsta = crr_cprilnksta_class_tbl[newcprista].hdlcsta;
	cmw_cprilnkstatbl[linkno].stafflg = crr_cprilnksta_class_tbl[newcprista].stafflg;
	cmw_cprilnkstatbl[linkno].L2abnminfo =	crr_cprilnksta_class_tbl[newcprista].L2abnminfo;

	if (newcprista == CRD_CPRILNKSTA_NO0)
	{  /* this case is for E down. In case of  E down, CPRI state has state A/B/C/D 4 value  */
	
		cmw_cprilnkstatbl[linkno].cprista = cpristate;
	}

	cm_Assert( CMD_ASL_DBGLOW, (UINT)((UINT)(linkno<<24) | (UINT)(linkevent<<16) | (UINT)(newcprista << 8) | (UINT)cprilnkstano ), 
						"cpri state mng change" );
	return;
}
/* @} */

/* @} */  
