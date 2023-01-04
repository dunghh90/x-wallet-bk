/*!
 * @skip   $Id$
 * @file   m_dl_cm_FlashProSnd.c
 * @brief  the processing for sending writing/erasing FLASH request to TDL.
 * @date   2009/04/30 FFCS)Wuh Create for eNB-008-001.
 * @date   2010/09/24  FJT)Taniguchi modify for M-S3G-eNBSYS-01991
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2009
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"															/* MDIF common head file			  */
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing function for sending writing/erasing FLASH request to pf_fmng
 *  @note   Call API to send the Writing/Erasing FLASH request to pf_fmng task.
 *          1) judge the process kind(erasing or writing)
 *          2) call API to send message to pf_fmng task 
 *  @param  a_sys_typ		 	    [in]  the type of REC system send to(3G or S3G)
 *  @param  a_pro_typ		 	    [in]  the process type:writing or erasing
 *  @param  a_down_typ              [in]  the type of download:donwnload in startup state or download in USE state.	
 *  @return Void.
 *  @retval -
 *  @date   2009/04/30 FFCS)Wuh Create.
 *  @date   2010/09/24  FJT)Taniguchi modify for M-S3G-eNBSYS-01991
  *  @date   2013/11/13  FFCS)linb Modify for DCM Zynq
 */
VOID m_dl_cm_FlashProSnd( USHORT a_sys_typ, USHORT a_pro_typ, USHORT a_down_typ )
{
	INT		qid		= D_RRH_PROCQUE_L3;
	INT		wtime	= CMD_NUM0;
	INT		ret		= CMD_NUM0;
	UINT 	side 	= CMD_NUM0;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_cm_FlashProSnd] ENTER" );		
	
	/** DCM Zynq: call API to pf_fmng for erasing/writing **/
	if( a_sys_typ == CMD_SYS_S3G )
	{
		/** erasing **/
		if( a_pro_typ == CMD_ERASE_TYP )
		{
			ret = rrhApi_File_Mnr_EraseFirm( qid, wtime, NULL, side);
			if(ret != E_API_RCD_OK)
			cm_Assert(CMD_ASL_USELOW, ret, "rrhApi_File_Mnr_EraseFirm NG");
		}
		/** writing **/
		else
		{
			/** Assert **/
			cm_Assert(CMD_ASL_USELOW, a_pro_typ, "process type invalid");
		}
		
	}
	else
	{
		/** Assert **/
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "system type invalid(%d)", a_sys_typ);
	}

	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_cm_FlashProSnd] RETURN" );		
	return;
}
/* @} */
