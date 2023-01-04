/*!
 * @skip   $Id$
 * @file   m_dl_cm_DlEnd.c
 * @brief  clear the tables for DLM thread.
 * @date   2008/07/22 FFCS)Wuh Create for eNB-008-001.
 * @date   2008/08/05 FFCS)Wuh Delete the of_typ parameter of cmw_re_dev_tbl.
 * @date   2009/04/13  FFCS)Wuh modify for S3G PRC CR-00038-005 (CPRI spec V1.07)
 * @date   2009/06/15  FFCS)Tangj modify for M-S3G-eNBPF-02080
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"			/* MDIF common head file			*/
#include "m_dl_header.h"			/* download management task head file */

/*!
 *  @brief  the function for tables clearing.
 *  @note   This function is processed as follows.
 *          1) Clear the tables defined in DLM thread
 *  @param  a_sys_typ [in]  the type of the system of REC,3G or S3G
 *  @return Void.
 *  @retval -
 *  @date   2008/07/22 FFCS)Wuh Create.
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq.
 */

VOID m_dl_cm_DlEnd( USHORT a_sys_typ )
{
	/****************/
	/* tables clear */
	/****************/
	
	/* file information report notification's target is systemparameter */
	if( cmw_file_inf_tbl[a_sys_typ].target_data == CMD_SYSPAR )
	{
		/* receiving data table clearing(the systemparameter's size is 32 byte) */
        if(a_sys_typ == CMD_SYS_3G)
        {
		    cm_MemClr( dlw_rcvsyspa_tbl_3g, dlr_syssize_settbl[a_sys_typ]);
        }
		/* receiving data table clearing(the systemparameter's size is 64 byte) */
		else
		{
            cm_MemClr( dlw_rcvsyspa_tbl_s3g, dlr_syssize_settbl[a_sys_typ]);
		}
	}
	/* set the download type no download */
	if(cmw_file_inf_tbl[a_sys_typ].target_data == CMD_FRM)
	{
		dlw_downtyp_tbl = DLD_TSK_TYP_NONE;
	}
	/* clear the file information report notification table */
	cm_MemClr( &cmw_file_inf_tbl[a_sys_typ], sizeof( cmw_file_inf_tbl[a_sys_typ] ));

	/* clear download management table */
	cm_MemClr( &dlw_dlmng_tbl[a_sys_typ], sizeof( dlw_dlmng_tbl[a_sys_typ] ));

	/* reset the handler */
	dlw_handle = CMD_NUM0;
	
	/* clear timer management table */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	
	return ;
}

/* @} */

