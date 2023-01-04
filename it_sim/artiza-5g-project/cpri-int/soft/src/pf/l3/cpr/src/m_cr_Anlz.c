/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_Anlz.c
 *  @brief  event analyze function
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
* @brief event analyze function
* @note index task internal event through global event code .\n
* @param src_id [in] source task id
* @param event_no [in] external event id
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
*
*/

UINT m_cr_Anlz(UINT src_id, UINT event_no)
{
    UINT returncode;	/* function return code */
	USHORT tblidx;		/* event table index */

 	returncode = CMD_NG;
	tblidx = CMD_NUM0;

	while(crr_event_ctbl[tblidx].gbleventno != CRD_ECTBL_END)
	{
        if (event_no == crr_event_ctbl[tblidx].gbleventno)
		{
        	crw_taskevent = crr_event_ctbl[tblidx].intereventno;
            returncode = CMD_OK;        /*  normal end */
	        break;
		}
	    else
		{
			tblidx++;                    /*  next index */
        }
	}

	return (returncode);
}
/* @} */

/* @} */
